#ifndef HPARALIBCONTROLLER_H
#define HPARALIBCONTROLLER_H


class HParaLibController
{
public:
    static HParaLibController* GetInstance();
    static void Release();

    enum ParaCutter{

        Cutter_General_focus,
        Cutter_General_count,

        Cutter_Laser_power,
        Cutter_Laser_prf,
        Cutter_Laser_id1,
        Cutter_Laser_id2,
        Cutter_Laser_id3,
        Cutter_Laser_psn,
        Cutter_Laser_dpk,

        Cutter_Cam_speed,
        Cutter_Cam_acc,
        Cutter_Cam_jumpSpeed,
        Cutter_Cam_jumpAcc,
        Cutter_Cam_jumpDelay,
        Cutter_Cam_trigDistance,
        Cutter_Cam_pulseWidth,
        Cutter_Cam_dutyPercent,
        Cutter_Cam_pulseCount,
        Cutter_Cam_pulseDelay,

    };

    enum ParaScanner{

        Scanner_General_focus,
        Scanner_General_count,

        Scanner_Laser_power,
        Scanner_Laser_prf,
        Scanner_Laser_pulse,
        Scanner_Laser_fpk,

        Scanner_Cam_speed,
        Scanner_Cam_acc,
        Scanner_Cam_jumpSpeed,
        Scanner_Cam_jumpAcc,
        Scanner_Cam_jumpDelay,
        Scanner_Cam_markDelay,
        Scanner_Cam_polyDelay,
        Scanner_Cam_openLaserDelay,
        Scanner_Cam_closeLaserDelay,

    };

    bool Init();

    QStringList getParaDatas(qint32 mode,const QString & name=nullptr) const;
    QStringList getGeneralDatas(qint32 mode,const QString & name=nullptr) const;
    QStringList getLaserDatas(qint32 mode,const QString & name=nullptr) const;
    QStringList getCamDatas(qint32 mode,const QString & name=nullptr) const;

    const QStringList & getNames(qint32 mode) const{
        switch (mode) {
        case 0:
            return m_names_cutter;
        case 1:
            return m_names_scanner;
        default:
            return m_names_cutter;
        }
    }
    const QString & getDefaultName(qint32 mode) const{
        switch (mode) {
        case 0:
            return m_defaultCutter;
        case 1:
            return m_defaultScanner;
        default:
            return m_defaultCutter;
        }
    }

    qint32 getDefaultIndex(qint32 mode) const{
        switch (mode) {
        case 0:
            return m_names_cutter.indexOf(m_defaultCutter);
        case 1:
            return m_names_scanner.indexOf(m_defaultScanner);
        default:
            return -1;
        }
    }

    void setDefaultName(qint32 mode,const QString &name) {
        switch (mode) {
        case 0:
            m_defaultCutter=name;
            break;
        case 1:
            m_defaultScanner=name;
            break;
        default:
            break;
        }
    }

    void setDefaultCutterIndex(qint32 mode,qint32 index) {
        switch (mode) {
        case 0:
            if(index>-1&&index<m_names_cutter.size())m_defaultCutter=m_names_cutter.value(index);
            break;
        case 1:
            if(index>-1&&index<m_names_scanner.size())m_defaultScanner=m_names_scanner.value(index);
            break;
        default:
            break;
        }
    }


    QStringList  getDatas(qint32 mode,QString name=nullptr) const;

    QStringList  getDatas(qint32 mode,qint32 index) const;

    void addNew(qint32 mode,const QString &name,const QStringList& datas);


    void deletePara(qint32 mode,const QString &name);
    void deletePara(qint32 mode,qint32 index);

    void clearParas(qint32 mode);

    void setData(qint32 mode,const QString &name,qint32 index,const QString& data);

    void setDatas(qint32 mode,const QString &name,const QStringList& datas );

    void setDatas(qint32 mode,qint32 index,const QStringList& datas );



    friend QDataStream &operator>>(QDataStream & input, HParaLibController &obj);
    friend QDataStream &operator<<(QDataStream & output, const HParaLibController &obj);





private:
    explicit HParaLibController();
    ~HParaLibController();
    static HParaLibController* m_pInstance;

    inline QStringList getParas(qint32 mode,QString name,qint32 start,qint32 end) const;

    bool writeToFile(const QString &filePath);
    bool openFromFile(const QString &filePath);

    QString filePath;


    QMap<QString,QStringList> m_data_cutter;
    QMap<QString,QStringList> m_data_scanner;

    QStringList m_names_cutter;
    QStringList m_names_scanner;

    QString m_defaultCutter;
    QString m_defaultScanner;
};

#endif // HPARALIBCONTROLLER_H
