#ifndef DIALOGEXPORT_H
#define DIALOGEXPORT_H

#include <QDialog>

namespace Ui {
class DialogExport;
}

class DialogExport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExport(QWidget *parent = nullptr);
    ~DialogExport();

    void setData(const QStringList &headers,const QVector<QStringList> &datas);

    friend QDataStream &operator>>(QDataStream & input, DialogExport &obj);
    friend QDataStream &operator<<(QDataStream & output, const DialogExport &obj);


private slots:
    void on_toolButton_clicked();

private:

    QStringList getControlData() const;
    void setControlData(const QStringList& data);

    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);


    Ui::DialogExport *ui;
    QVector<QStringList> m_data;
};

#endif // DIALOGEXPORT_H
