#ifndef H_PROCESSWIDGET_H
#define H_PROCESSWIDGET_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QLayout;
class QLineEdit;
QT_END_NAMESPACE

class ProgramHandler;
class HProperty_para;
class HGraphicsItem;
class IniSettings;


class H_ProcessWidget : public QWidget
{
    Q_OBJECT
public:
    static H_ProcessWidget* GetInstance(QWidget *parent = nullptr);

private:
    explicit H_ProcessWidget(QWidget *parent = nullptr);
    static H_ProcessWidget* m_pInstance;
    ~H_ProcessWidget() Q_DECL_OVERRIDE;

   inline  HProperty_para* getGeneralWid(qint32 mode) const;
   inline  HProperty_para* getLaserWid(qint32 mode) const;
   inline  HProperty_para* getCamWid(qint32 mode) const;

signals:


public slots:

private slots:
    void saveItemData(const QString &name,qint32 type);


private:
    //    void addLaserWidget();
    //    void addMotionWidget();
    //    void addObjectWidget();


    //    QLayout* addLabelLineEdit(const QStringList &texts, QVector<QLineEdit*> &lineEdits, QWidget *parent=nullptr);

    //    QVector<QLineEdit*> ptrLineEdits_laser;
    //    QVector<QLineEdit*> ptrLineEdits_motion;


    QTabWidget *ptrTabWidget;
    ProgramHandler *ptrdHandler;
    IniSettings *ptrIniSettings;

    QList<QGraphicsItem *> selectedItems;


    HProperty_para* ptrGeneral1Wid;
    HProperty_para* ptrGeneral2Wid;
    HProperty_para* ptrLaser1Wid;
    HProperty_para* ptrLaser2Wid;
    HProperty_para* ptrCam1Wid;
    HProperty_para* ptrCam2Wid;
    QPushButton *captureButton;
    HProperty_para* ptrLocationWid;
    QMap<qint32,HProperty_para*> itemParaWids;


    //    typedef  std::pair<H_Wid_Common*,ParaWidgetType>  ItemTabWidget;
    //    typedef QVector<ItemTabWidget> ItemWidgets;
    //    QHash<HItemSpace::ItemType,ItemWidgets> AllItemWidgets;




};

#endif // H_PROCESSWIDGET_H
