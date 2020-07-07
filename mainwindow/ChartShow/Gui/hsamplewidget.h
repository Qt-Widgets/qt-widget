#ifndef HSAMPLEWIDGET_H
#define HSAMPLEWIDGET_H

#include <QDialog>

class QLineEdit;
class QBoxLayout;
class QPushButton;
class QLabel;

enum SampleParaType{
    SAMPLE_INTERVAL,
    SAMPLE_X_MIN,
    SAMPLE_X_MAX,
    SAMPLE_Y_MIN,
    SAMPLE_Y_MAX
};

class HSampleWidget : public QDialog
{
    Q_OBJECT
public:

    static HSampleWidget* GetInstance(QWidget *parent = nullptr);


    qreal GetPara(SampleParaType type);

signals:

    void ParaSaved();

public slots:

private slots:
    void ButtonSaveHandler();

private:

    explicit HSampleWidget(QWidget *parent = nullptr,Qt::WindowFlags f =Qt::WindowCloseButtonHint);
    static HSampleWidget* m_pInstance;

    QVector<QLineEdit *> lineEditors;
    void CreatLineEditor1(QVector<QLineEdit *> &lineVector,QWidget *parentWid, QBoxLayout *parentLay, QString name);
    void CreatLineEditor2(QVector<QLineEdit *> &lineVector,QWidget *parentWid, QBoxLayout *parentLay, QString name);
};

#endif // HSAMPLEWIDGET_H
