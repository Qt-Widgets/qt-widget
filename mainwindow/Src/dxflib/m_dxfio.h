#ifndef M_DXFIO_H
#define M_DXFIO_H

#include "dl_creationadapter.h"
#include <QList>
#include <QMutex>
#include <QColor>


struct Data_Layer{
    QString name="0";
    QColor color=Qt::black;
    int width=100;
    QString linetype="CONTINUOUS";
    int flags=0;
    bool isoff=false;
};

struct Data_Block{

    QString name;
    int flags;
    double bpx;
    double bpy;
    double bpz;
};

struct Data_Point{
    double x;
    double y;
    double z=0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";

};

struct Data_FitPoint{
    double x;
    double y;
    double z=0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";

};

struct Data_Line{
    double x1;
    double y1;
    double z1=0.0;
    double x2;
    double y2;
    double z2=0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_XLine{
    double bx;
    double by;
    double bz=0.0;
    double dx;
    double dy;
    double dz=0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Spline{

    unsigned int degree;
    unsigned int nKnots;
    unsigned int nControl;
    unsigned int nFit;
    int flags;

    double tangentStartX;
    double tangentStartY;
    double tangentStartZ;
    double tangentEndX;
    double tangentEndY;
    double tangentEndZ;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Ray{
    double bx;
    double by;
    double bz=0.0;
    double dx;
    double dy;
    double dz=0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Arc{
    double cx;
    double cy;
    double cz=0.0;
    double radius;
    double angle1;
    double angle2;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};
struct Data_Circle{
    double cx;
    double cy;
    double cz=0.0;
    double radius;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Ellipse{

    double cx;
    double cy;
    double cz;
    double mx;
    double my;
    double mz;

    double ratio;
    double angle1;
    double angle2;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Insert{

    QString name;
    double ipx;
    double ipy;
    double ipz;
    double sx;
    double sy;
    double sz;
    double angle;
    int cols;
    int rows;
    double colSp;
    double rowSp;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

struct Data_Polyline{
    int flags;
    uint number;
    uint mverteces;
    uint nverteces;
    double pElevation = 0.0;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};
struct Data_Vertex{
    double x;
    double y;
    double z=0.0;
    double bulge;

};
struct Data_MText{

    double ipx;
    double ipy;
    double ipz;
    double dirx;
    double diry;
    double dirz;
    double height;
    double width;
    int attachmentPoint;
    int drawingDirection;
    int lineSpacingStyle;
    double lineSpacingFactor;
    QString text;
    QString style;
    double angle;

};

struct Data_Text{

    double ipx;
    double ipy;
    double ipz;

    double apx;
    double apy;
    double apz;
    double height;
    double xScaleFactor;
    int textGenerationFlags;

    int hJustification;
    int vJustification;
    QString text;
    QString style;
    double angle;

};
struct Data_ArcAlignedText{

    QString text;
    QString font;
    QString style;

    double cx;
    double cy;
    double cz;
    double radius;


    double xScaleFactor;
    double height;
    double spacing;
    double offset;
    double rightOffset;
    double leftOffset;
    double startAngle;
    double endAngle;
    bool reversedCharacterOrder;
    int direction;
    int alignment;
    int side;
    bool bold;
    bool italic;
    bool underline;
    int characerSet;
    int pitch;

    bool shxFont;
    bool wizard;
    int arcHandle;

};
struct Data_3dFace{
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    double x3;
    double y3;
    double z3;
    double x4;
    double y4;
    double z4;

    QString layer="0";
    int color=DL_Codes::black;
    int width=100;
    QString linetype="CONTINUOUS";
};

typedef QVector<Data_Layer> DXF_Layer;
typedef QVector<Data_Block> DXF_Block;
typedef QVector<Data_Point> DXF_Point;
typedef QVector<Data_Line> DXF_Line;
typedef QVector<Data_XLine> DXF_XLine;
typedef QVector<Data_Spline> DXF_Spline;
typedef QVector<Data_Ray> DXF_Ray;
typedef QVector<Data_Arc> DXF_Arc;
typedef QVector<Data_Circle> DXF_Circle;
typedef QVector<Data_Ellipse> DXF_Ellipse;
typedef QVector<Data_Insert> DXF_Insert;
typedef QVector<Data_Polyline> DXF_Polyline;
typedef QVector<Data_Vertex> DXF_Vertex;
typedef QVector<Data_3dFace> DXF_3dFace;
typedef QVector<Data_Text> DXF_Text;
typedef QVector<Data_ArcAlignedText> DXF_ArcAlignedText;


class M_dxfIO : public DL_CreationAdapter
{

public:
    static M_dxfIO* GetInstance();
    static void Release();

    void freeAllData();
    bool readDxf(const QString& filepath);
    bool writeDxf(const QString& filepath);


    DXF_Layer *list_Layer;
    DXF_Block *list_Block;
    DXF_Point* list_Point;
    DXF_Line* list_Line;
    DXF_XLine* list_XLine;
    DXF_Spline* list_Spline;
    DXF_Ray* list_Ray;
    DXF_Arc* list_Arc;
    DXF_Circle* list_Circle;
    DXF_Ellipse* list_Ellipse;
    DXF_Insert* list_Insert;
    DXF_Polyline* list_Polyline;
    DXF_Vertex* list_Vertex;
    DXF_3dFace* list_3dFace;
    DXF_Text* list_Text;
    DXF_ArcAlignedText* list_ArcAlignedText;


private:
    explicit M_dxfIO();
    ~M_dxfIO();
    static M_dxfIO* m_pInstance;



    virtual void addLayer(const DL_LayerData& data);
    virtual void addBlock(const DL_BlockData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addXLine(const DL_XLineData &data);
    virtual void addSpline(const DL_SplineData &data);
    virtual void addRay(const DL_RayData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addEllipse(const DL_EllipseData& data);
    virtual void addInsert(const DL_InsertData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void add3dFace(const DL_3dFaceData& data);
    virtual void addText(const DL_TextData& data);
    virtual void addArcAlignedText(const DL_ArcAlignedTextData& data);



};

#endif // M_DXFIO_H
