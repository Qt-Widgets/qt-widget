
#include "hmodbusmainwindow.h"
#include "ui_hmodbusmainwindow.h"
#include "settingsdialog.h"
#include "writeregistermodel.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>
#include <Src/data/inisettings.h>
#include <QTimer>

enum ModbusConnection {
    Serial,
    Tcp
};

HModBusMainWindow* HModBusMainWindow::m_pInstance = nullptr;

HModBusMainWindow *HModBusMainWindow::GetInstance(QWidget *parent)
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new HModBusMainWindow(parent);
    }
    return m_pInstance;
}

HModBusMainWindow::HModBusMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HModBusMainWindow)
    , lastRequest(nullptr)
    , modbusDevice(nullptr)
{
    ui->setupUi(this);

    ptrIniSettings=IniSettings::GetInstance();

    m_settingsDialog = new SettingsDialog(this);

    initActions();

    ui->writeAddress->setValue(ptrIniSettings->getValue(IniSettings::ModBus_Client_Write_AddrStart).toInt());
    ui->writeSize->setValue(ptrIniSettings->getValue(IniSettings::ModBus_Client_Write_Count).toInt());
    writeModel = new WriteRegisterModel(this,ui->writeSize->value());
    writeModel->setStartAddress(ui->writeAddress->value());
    writeModel->setNumberOfValues(ui->writeSize->value());

    ui->writeValueTable->setModel(writeModel);
    ui->writeValueTable->hideColumn(2);
    connect(writeModel, &WriteRegisterModel::updateViewport, ui->writeValueTable->viewport(),
            static_cast<void (QWidget::*)()>(&QWidget::update));

    ui->writeTable->addItem(tr("Coils"), QModbusDataUnit::Coils);
    ui->writeTable->addItem(tr("Discrete Inputs"), QModbusDataUnit::DiscreteInputs);
    ui->writeTable->addItem(tr("Input Registers"), QModbusDataUnit::InputRegisters);
    ui->writeTable->addItem(tr("Holding Registers"), QModbusDataUnit::HoldingRegisters);
    ui->writeTable->setCurrentIndex(ptrIniSettings->getValue(IniSettings::ModBus_Client_SigType).toInt());

    ui->connectType->setCurrentIndex(ptrIniSettings->getValue(IniSettings::ModBus_Client_ConnectType).toInt());
    on_connectType_currentIndexChanged(ptrIniSettings->getValue(IniSettings::ModBus_Client_ConnectType).toInt());

    auto valueChanged = static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged);
    connect(ui->writeSize,valueChanged, writeModel,&WriteRegisterModel::setNumberOfValues);
    connect(ui->writeAddress, valueChanged, writeModel, &WriteRegisterModel::setStartAddress);

    //    connect(ui->writeAddress, valueChanged, this, [this, model](int i) {
    //        int lastPossibleIndex = 0;
    //        const int currentIndex = ui->writeSize->currentIndex();
    //        for (int ii = 0; ii < 10; ++ii) {
    //            if (ii < (10 - i)) {
    //                lastPossibleIndex = ii;
    //                model->item(ii)->setEnabled(true);
    //            } else {
    //                model->item(ii)->setEnabled(false);
    //            }
    //        }
    //        if (currentIndex > lastPossibleIndex)
    //            ui->writeSize->setCurrentIndex(lastPossibleIndex);
    //    });


    //ui->connectButton->click();
    statusBar()->setStyleSheet("color:red");

    ui->readSize->setValue(ptrIniSettings->getValue(IniSettings::ModBus_Client_Read_Count).toInt());
    ui->readAddress->setValue(ptrIniSettings->getValue(IniSettings::ModBus_Client_Read_AddrStart).toInt());
    ui->serverEdit->setValue(ptrIniSettings->getValue(IniSettings::ModBus_Client_ServerAddr).toInt());

    connect(ui->autoConnect,&QCheckBox::stateChanged,this,[=](int checked){
        if(checked&&!connected){
            ui->connectButton->click();
        }
    });

    ui->autoConnect->setChecked(ptrIniSettings->getValue(IniSettings::ModBus_Client_AutoConnect).toBool());


}

HModBusMainWindow::~HModBusMainWindow()
{

    saveWidgetData();
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;

    delete ui;
}

void HModBusMainWindow::saveWidgetData()
{
    ptrIniSettings->setValue(IniSettings::ModBus_Client_ConnectType,ui->connectType->currentIndex());
    ptrIniSettings->setValue(ui->connectType->currentIndex()==Serial?IniSettings::ModBus_Client_ComName:IniSettings::ModBus_Client_IP,ui->portEdit->text());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_ServerAddr,ui->serverEdit->value());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_SigType,ui->writeTable->currentIndex());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_Read_AddrStart,ui->readAddress->value());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_Read_Count,ui->readSize->value());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_Write_AddrStart,ui->writeAddress->value());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_Write_Count,ui->writeSize->value());
    ptrIniSettings->setValue(IniSettings::ModBus_Client_AutoConnect,ui->autoConnect->isChecked());

}


qint32 HModBusMainWindow::GetHoldingValue(qint32 addr)
{

    readConpleted=false;

    if (!modbusDevice)
        return -1;

    if (auto *reply = modbusDevice->sendReadRequest(QModbusDataUnit(QModbusDataUnit::RegisterType(ui->writeTable->currentIndex()), addr, 1), ui->serverEdit->value())) {


        if (!reply->isFinished()){

            connect(reply, &QModbusReply::finished, this, &HModBusMainWindow::readReady);

            qint32 ct=0;
            while (!readConpleted) {

                if(ct==11){
                    return -1;
                }
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit()));
                loop.exec();
                ++ct;
            }
            return readResult;
        }
        else{
            delete reply; // broadcast replies return immediately
            return -1;
        }

    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(),5000);
        return -1;
    }


}

bool HModBusMainWindow::SetHoldingValue(qint32 addr, qint32 value)
{
    if (!modbusDevice)
        return false;


    //QModbusDataUnit::RegisterType table = writeUnit.registerType();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::RegisterType(ui->writeTable->currentIndex()), addr, 1);
    writeUnit.setValue(0,value);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, ui->serverEdit->value())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),5000);
                    writeConpleted=true;
                    writeResult=false;
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16),5000);
                    writeConpleted=true;
                    writeResult=false;
                }
                reply->deleteLater();
                writeConpleted=true;
                writeResult=true;
            });

            qint32 ct=0;
            while (!writeConpleted) {

                if(ct==11){
                    return false;
                }
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit()));
                loop.exec();
                ++ct;
            }
            return writeResult;


        } else {
            // broadcast replies return immediately
            reply->deleteLater();
            return false;
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(),5000);
        return false;
    }

}

void HModBusMainWindow::initActions()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionOptions->setEnabled(true);

    connect(ui->actionConnect, &QAction::triggered,
            this, &HModBusMainWindow::on_connectButton_clicked);
    connect(ui->actionDisconnect, &QAction::triggered,
            this, &HModBusMainWindow::on_connectButton_clicked);

    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, m_settingsDialog, &QDialog::show);
}


void HModBusMainWindow::on_connectType_currentIndexChanged(int index)
{
    connectionTypeNoChanged=false;
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection> (index);
    if (type == Serial) {
        modbusDevice = new QModbusRtuSerialMaster(this);
        ui->portEdit->setInputMask("");
        ui->portEdit->setText(ptrIniSettings->getValue(IniSettings::ModBus_Client_ComName).toString());
    } else if (type == Tcp) {
        modbusDevice = new QModbusTcpClient(this);
        ui->portEdit->setText(ptrIniSettings->getValue(IniSettings::ModBus_Client_IP).toString());
    }



    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {
        ui->connectButton->setDisabled(true);
        if (type == Serial)
            statusBar()->showMessage(tr("Could not create Modbus master."), 5000);
        else
            statusBar()->showMessage(tr("Could not create Modbus client."), 5000);
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &HModBusMainWindow::onStateChanged);
        if(ui->autoConnect->isChecked())
            ui->connectButton->click();
    }
}

void HModBusMainWindow::on_connectButton_clicked()
{
    if (!modbusDevice)
        return;

    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (static_cast<ModbusConnection> (ui->connectType->currentIndex()) == Serial) {
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                                 ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                                 m_settingsDialog->settings().parity);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                                 m_settingsDialog->settings().baud);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                                 m_settingsDialog->settings().dataBits);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                                 m_settingsDialog->settings().stopBits);
        } else {
            const QUrl url = QUrl::fromUserInput(ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        }
        modbusDevice->setTimeout(m_settingsDialog->settings().responseTime);
        modbusDevice->setNumberOfRetries(m_settingsDialog->settings().numberOfRetries);
        if (!modbusDevice->connectDevice()) {
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        } else {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
    } else {
        modbusDevice->disconnectDevice();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }
}

void HModBusMainWindow::onStateChanged(int state)
{
    connected = (state != QModbusDevice::UnconnectedState);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState){
        ui->connectButton->setText(tr("Connect"));

        if(ui->autoConnect->isChecked()&&connectionTypeNoChanged){

            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();
            ui->connectButton->click();
            connectionTypeNoChanged=true;
        }
    }
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));

}

void HModBusMainWindow::on_readButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

    if (auto *reply = modbusDevice->sendReadRequest(readRequest(), ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &HModBusMainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void HModBusMainWindow::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("Address: %1, Value:   %2").arg(unit.startAddress() + i)
                    .arg(QString::number(unit.value(i),
                                         unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
            readResult=unit.value(i);
            ui->readValue->addItem(entry);
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        statusBar()->showMessage(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                 arg(reply->errorString()).
                                 arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        statusBar()->showMessage(tr("Read response error: %1 (code: 0x%2)").
                                 arg(reply->errorString()).
                                 arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
    readConpleted=true;
}

void HModBusMainWindow::on_writeButton_clicked()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    for (quint32 i = 0; i < writeUnit.valueCount(); i++) {
        if (table == QModbusDataUnit::Coils)
            writeUnit.setValue(i, writeModel->m_coils[i ]);
        else
            writeUnit.setValue(i, writeModel->m_holdingRegisters[i]);
    }

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, ui->serverEdit->value())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        statusBar()->showMessage(tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}

void HModBusMainWindow::on_readWriteButton_clicked()
{
    if (!modbusDevice)
        return;
    ui->readValue->clear();
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = writeRequest();
    QModbusDataUnit::RegisterType table = writeUnit.registerType();
    for (uint i = 0; i < writeUnit.valueCount(); i++) {
        if (table == QModbusDataUnit::Coils)
            writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
        else
            writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
    }

    if (auto *reply = modbusDevice->sendReadWriteRequest(readRequest(), writeUnit,
                                                         ui->serverEdit->value())) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &HModBusMainWindow::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        statusBar()->showMessage(tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void HModBusMainWindow::on_writeTable_currentIndexChanged(int index)
{
    const bool coilsOrHolding = index == 0 || index == 3;
    if (coilsOrHolding) {
        ui->writeValueTable->setColumnHidden(1, index != 0);
        ui->writeValueTable->setColumnHidden(2, index != 3);
        ui->writeValueTable->resizeColumnToContents(0);
    }

    ui->readWriteButton->setEnabled(index == 3);
    ui->writeButton->setEnabled(coilsOrHolding);
    ui->writeGroupBox->setEnabled(coilsOrHolding);
}

QModbusDataUnit HModBusMainWindow::readRequest() const
{
    const auto table =
            static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());

    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    return QModbusDataUnit(table, ui->readAddress->value(), static_cast<quint16>(ui->readSize->value()));
}

QModbusDataUnit HModBusMainWindow::writeRequest() const
{
    const auto table =
            static_cast<QModbusDataUnit::RegisterType> (ui->writeTable->currentData().toInt());


    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    return QModbusDataUnit(table, ui->writeAddress->value(), static_cast<quint16>(ui->writeSize->value()));
}
