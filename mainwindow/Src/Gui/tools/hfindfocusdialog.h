#ifndef HFINDFOCUSDIALOG_H
#define HFINDFOCUSDIALOG_H

#include <QDialog>

namespace Ui {
class HFindFocusDialog;
}

class HParaLibController;
class H_MotionController;

class HFindFocusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HFindFocusDialog(QWidget *parent = nullptr);
    ~HFindFocusDialog() override;

    friend QDataStream &operator>>(QDataStream & input, HFindFocusDialog &obj);
    friend QDataStream &operator<<(QDataStream & output, const HFindFocusDialog &obj);

private slots:
    void on_toolButton_cut_clicked();

    void on_toolButton_save_clicked();

protected:
    void showEvent(QShowEvent *event) override;

private:
    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);


    Ui::HFindFocusDialog *ui;

    qint32 data_mode=0;
    HParaLibController *ptrParaLib;
    H_MotionController *ptrMotion;

    //control data
    QString m_axisX;
    QString m_axisY;
    QString m_axisZ;
    QPointF m_position;
    bool m_useMark;
    QString m_number;
    QString m_step;
    qint32 m_markType;
    QString m_markSize;
    QString m_markInterval;
};

#endif // HFINDFOCUSDIALOG_H
