#include "m_dxfio.h"
#include "dl_dxf.h"
#include <QDebug>



M_dxfIO* M_dxfIO::m_pInstance = nullptr;

M_dxfIO *M_dxfIO::GetInstance()
{
    if(m_pInstance == nullptr)
    {
        m_pInstance = new M_dxfIO();
        atexit(Release);
    }
    return m_pInstance;
}
void M_dxfIO::Release()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void M_dxfIO::freeAllData()
{
    list_Layer->clear();
    list_Block->clear();
    list_Point->clear();
    list_Line->clear();
    list_XLine->clear();
    list_Spline->clear();
    list_Ray->clear();
    list_Arc->clear();
    list_Circle->clear();
    list_Ellipse->clear();
    list_Insert->clear();
    list_Polyline->clear();
    list_Vertex->clear();
    list_3dFace->clear();
    list_Text->clear();
    list_ArcAlignedText->clear();
}



static QColor colorToQt(int dxfcolor){

    switch (dxfcolor) {
    case 250:
        return Qt::black;
    case 3:
        return Qt::green;
    case 1:
        return Qt::red;
    case 15:
        return QColor(165,42,42);
    case 2:
        return Qt::yellow;
    case 4:
        return Qt::cyan;
    case 6:
        return QColor(255,0,255);
    case 8:
        return Qt::gray;
    case 5:
        return Qt::blue;
    case 163:
        return Qt::blue;
    case 121:
        return Qt::green;
    case 131:
        return Qt::cyan;
    case 23:
        return Qt::red;
    case 221:
        return QColor(255,0,255);
    case 252:
        return Qt::gray;
    case 7:
        return Qt::white/*BLACK TO WHITE*/;
    default:
        return Qt::white;
    }
}

bool M_dxfIO::readDxf(const QString &filepath)
{
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(filepath.toStdWString().c_str(), this)) {
        delete dxf;
        return false;
    }
    delete dxf;
    return true;
}

bool M_dxfIO::writeDxf(const QString &filepath)
{
    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out(filepath.toStdString().c_str(), exportVersion);

    if (dw==nullptr) {
        delete dxf;
        return false;
    }
    dxf->writeHeader(*dw);

    dw->sectionEnd();
    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(3);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dw->tableEnd();

    dw->tableLayers(list_Layer->size());


    //    DXF_Layer::Iterator layer;
    //    for (layer = list_Layer->begin();layer != list_Layer->end(); ++layer) {
    //        dxf->writeLayer(*dw,DL_LayerData(layer.value().name.toStdString(), layer.value().flags,layer.value().isoff),
    //                        DL_Attributes(
    //                            layer.value().layer.toStdString(),      // leave empty
    //                            DL_Codes::black,        // default color
    //                            layer.value().width,                  // default width
    //                            layer.value().linetype.toStdString(), 1.0));       // default line style
    //    }

    //    DXF_Point::Iterator point;
    //    for (point = list_Point->begin();point != list_Point->end(); ++point) {
    //        dxf->writePoint(*dw,DL_PointData(point.value().x,point.value().y,point.value().z),
    //                        DL_Attributes(point.value().layer.toStdString(), point.value().color, point.value().width, point.value().linetype.toStdString(), 1.0));
    //    }



    //    DXF_Line::Iterator line;
    //    for (line = list_Line->begin();line != list_Line->end(); ++line) {
    //        dxf->writeLine(
    //                    *dw,
    //                    DL_LineData(line.value().x1,   // start point
    //                                line.value().y1,
    //                                line.value().z1,
    //                                line.value().x2,   // end point
    //                                line.value().x2,
    //                                line.value().x2),
    //                    DL_Attributes(line.value().layer.toStdString(), line.value().color, line.value().width,line.value().linetype.toStdString(), 1.0));
    //    }


    //    DXF_Arc::Iterator arc;
    //    for (arc = list_Arc->begin();arc != list_Arc->end(); ++arc) {
    //        dxf->writeArc(*dw,DL_ArcData(arc.value().cx,arc.value().cy,arc.value().cz,arc.value().radius,arc.value().angle1, arc.value().angle2),
    //                      DL_Attributes(arc.value().layer.toStdString(), arc.value().color, arc.value().width, arc.value().linetype.toStdString(),1.0));
    //    }


    //    DXF_Circle::Iterator circle;
    //    for (circle = list_Circle->begin();circle != list_Circle->end(); ++circle) {
    //        dxf->writeCircle(*dw,DL_CircleData(circle.value().cx,circle.value().cy,circle.value().cz,circle.value().radius),
    //                         DL_Attributes(circle.value().layer.toStdString(), circle.value().color, circle.value().width, circle.value().linetype.toStdString(),1.0));
    //    }


    //    DXF_Polyline::Iterator polyline;
    //    for (polyline = list_Polyline->begin();polyline != list_Polyline->end(); ++polyline) {
    //        dxf->writePolyline(*dw,DL_PolylineData(polyline.value().number,polyline.value().mverteces,polyline.value().nverteces,polyline.value().flags),
    //                           DL_Attributes(polyline.value().layer.toStdString(), polyline.value().color, polyline.value().width, polyline.value().linetype.toStdString(),1.0));
    //    }


    //    DXF_Vertex::Iterator vertex;
    //    for (vertex = list_Vertex->begin();vertex != list_Vertex->end(); ++vertex) {
    //        dxf->writeVertex(*dw,DL_VertexData(vertex.value().x,vertex.value().y,vertex.value().z,vertex.value().bulge));
    //    }


    //    DXF_3dFace::Iterator threedface;
    //    for (threedface = list_3dFace->begin();threedface != list_3dFace->end(); ++threedface) {
    //        dxf->write3dFace(*dw,DL_3dFaceData(threedface.value().x1,threedface.value().y1,threedface.value().z1,
    //                                           threedface.value().x2,threedface.value().y2,threedface.value().z2,
    //                                           threedface.value().x3,threedface.value().y3,threedface.value().z3,
    //                                           threedface.value().x4,threedface.value().y4,threedface.value().z4,0.0),
    //                         DL_Attributes(threedface.value().layer.toStdString(), threedface.value().color, threedface.value().width, threedface.value().linetype.toStdString(),1.0));
    //    }
    delete dxf;
    return true;
}

M_dxfIO::M_dxfIO()
{
    list_Layer =new DXF_Layer;
    list_Block=new DXF_Block;
    list_Point=new DXF_Point;
    list_Line=new DXF_Line;
    list_Spline=new DXF_Spline;
    list_XLine=new DXF_XLine;
    list_Arc=new DXF_Arc;
    list_Ray=new DXF_Ray;
    list_Insert=new DXF_Insert;
    list_Circle=new DXF_Circle;
    list_Ellipse=new DXF_Ellipse;
    list_Polyline=new DXF_Polyline;
    list_Vertex=new DXF_Vertex;
    list_3dFace=new DXF_3dFace;
    list_Text=new DXF_Text;
    list_ArcAlignedText=new DXF_ArcAlignedText;


}

M_dxfIO::~M_dxfIO()
{
    delete list_Layer;
    delete list_Block;
    delete list_Point;
    delete list_Line;
    delete list_Spline;
    delete list_XLine;
    delete list_Arc;
    delete list_Ray;
    delete list_Insert;
    delete list_Circle;
    delete list_Ellipse;
    delete list_Polyline;
    delete list_Vertex;
    delete list_3dFace;
    delete list_Text;
    delete list_ArcAlignedText;
}

void M_dxfIO::addLayer(const DL_LayerData& data) {

    Data_Layer km;
    km.color=colorToQt(attributes.getColor());
    km.width=attributes.getWidth();
    km.linetype=QString::fromStdString(attributes.getLinetype());
    km.flags=data.flags;
    km.name=QString::fromStdString(data.name);

    list_Layer->append(km);
}

void M_dxfIO::addBlock(const DL_BlockData &data)
{

    Data_Block km;

    km.bpx=data.bpx;
    km.bpy=data.bpy;
    km.bpz=data.bpz;
    km.flags=data.flags;
    km.name=QString::fromStdString(data.name);

    list_Block->append(km);


}


void M_dxfIO::addPoint(const DL_PointData& data) {

    Data_Point km;
    km.x=data.x;
    km.y=data.y;
    km.z=data.z;

    km.layer=QString::fromStdString(attributes.getLayer());
    km.color=attributes.getColor();
    km.width=attributes.getWidth();
    km.linetype=QString::fromStdString(attributes.getLinetype());

    list_Point->append(km);

}


void M_dxfIO::addLine(const DL_LineData& data) {
    Data_Line km;
    km.x1=data.x1;
    km.y1=data.y1;
    km.z1=data.z1;
    km.x2=data.x2;
    km.y2=data.y2;
    km.z2=data.z2;

    km.layer=QString::fromStdString(attributes.getLayer());
    km.color=attributes.getColor();
    km.width=attributes.getWidth();
    km.linetype=QString::fromStdString(attributes.getLinetype());

    list_Line->append(km);

}

void M_dxfIO::addXLine(const DL_XLineData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_XLine km;
    //    km.bx=data.bx;
    //    km.by=data.by;
    //    km.bz=data.bz;
    //    km.dx=data.dx;
    //    km.dy=data.by;
    //    km.dz=data.dz;

    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());
    //    list_XLine->insert(index,km);
    //    index++;
}

void M_dxfIO::addSpline(const DL_SplineData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_Spline km;
    //    km.degree=data.degree;
    //    km.nKnots=data.nKnots;
    //    km.nControl=data.nControl;
    //    km.nFit=data.nFit;
    //    km.flags=data.flags;

    //    km.tangentStartX=data.tangentStartX;
    //    km.tangentStartY=data.tangentStartY;
    //    km.tangentStartZ=data.tangentStartZ;
    //    km.tangentEndX=data.tangentEndX;
    //    km.tangentEndY=data.tangentEndY;
    //    km.tangentEndZ=data.tangentEndZ;

    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());
    //    list_Spline->insert(index,km);
    //    index++;
}

void M_dxfIO::addRay(const DL_RayData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_Ray km;
    //    km.bx=data.bx;
    //    km.by=data.by;
    //    km.bz=data.bz;

    //    km.dx=data.dx;
    //    km.dy=data.dy;
    //    km.dz=data.dz;

    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());
    //    list_Ray->insert(index,km);
    //    index++;

}


void M_dxfIO::addArc(const DL_ArcData& data) {

    Data_Arc km;
    km.cx=data.cx;
    km.cy=data.cy;
    km.cz=data.cz;
    km.radius=data.radius;
    km.angle1=data.angle1;
    km.angle2=data.angle2;


    km.layer=QString::fromStdString(attributes.getLayer());
    km.color=attributes.getColor();
    km.width=attributes.getWidth();
    km.linetype=QString::fromStdString(attributes.getLinetype());

    list_Arc->append(km);

}


void M_dxfIO::addCircle(const DL_CircleData& data) {

    Data_Circle km;
    km.cx=data.cx;
    km.cy=data.cy;
    km.cz=data.cz;
    km.radius=data.radius;

    km.layer=QString::fromStdString(attributes.getLayer());
    km.color=attributes.getColor();
    km.width=attributes.getWidth();
    km.linetype=QString::fromStdString(attributes.getLinetype());

    list_Circle->append(km);

}

void M_dxfIO::addEllipse(const DL_EllipseData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_Ellipse km;

    //    km.cx=data.cx;
    //    km.cy=data.cy;
    //    km.cz=data.cz;
    //    km.mx=data.mx;
    //    km.my=data.my;
    //    km.mz=data.mz;

    //    km.ratio=data.ratio;
    //    km.angle1=data.angle1;
    //    km.angle2=data.angle2;

    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());

    //    list_Ellipse->insert(index,km);
    //    index++;

}

void M_dxfIO::addInsert(const DL_InsertData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_Insert km;

    //    km.name=QString::fromStdString(data.name);
    //    km.ipx=data.ipx;
    //    km.ipy=data.ipy;
    //    km.ipz=data.ipz;
    //    km.sx=data.sx;
    //    km.sy=data.sy;
    //    km.sz=data.sz;
    //    km.angle=data.angle;
    //    km.cols=data.cols;
    //    km.rows=data.rows;
    //    km.colSp=data.colSp;
    //    km.rowSp=data.cols;

    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());

    //    list_Insert->insert(index,km);
    //    index++;
}


void M_dxfIO::addPolyline(const DL_PolylineData& data) {

        Data_Polyline km;
        km.flags=data.flags;
        km.number=data.number;
        km.mverteces=data.m;
        km.nverteces=data.n;
        km.pElevation=data.elevation;

        km.layer=QString::fromStdString(attributes.getLayer());
        km.color=attributes.getColor();
        km.width=attributes.getWidth();
        km.linetype=QString::fromStdString(attributes.getLinetype());


        list_Polyline->append(km);
    //    index++;
}



void M_dxfIO::addVertex(const DL_VertexData& data) {
    //    QMutexLocker locker(&m_mutex);
        Data_Vertex km;
        km.x=data.x;
        km.y=data.y;
        km.z=data.z;
        km.bulge=data.bulge;

        list_Vertex->append(km);
    //    index++;
}


void M_dxfIO::add3dFace(const DL_3dFaceData& data) {
    //    QMutexLocker locker(&m_mutex);

    //    Data_3dFace km;
    //    km.x1=data.x[0];
    //    km.y1=data.y[0];
    //    km.z1=data.z[0];

    //    km.x2=data.x[1];
    //    km.y2=data.y[1];
    //    km.z2=data.z[1];

    //    km.x3=data.x[2];
    //    km.y3=data.y[2];
    //    km.z3=data.z[2];

    //    km.x4=data.x[3];
    //    km.y4=data.y[3];
    //    km.z4=data.z[3];


    //    km.layer=QString::fromStdString(attributes.getLayer());
    //    km.color=attributes.getColor();
    //    km.width=attributes.getWidth();
    //    km.linetype=QString::fromStdString(attributes.getLinetype());

    //    list_3dFace->insert(index,km);
    //    index++;

}

void M_dxfIO::addText(const DL_TextData &data)
{
    //    QMutexLocker locker(&m_mutex);
    //    Data_Text km;

    //    km.ipx=data.ipx;
    //    km.ipy=data.ipy;
    //    km.ipz=data.ipz;

    //    km.apx=data.ipx;
    //    km.apy=data.ipy;
    //    km.apz=data.ipz;
    //    km. height=data.height;
    //    km.xScaleFactor=data.xScaleFactor;
    //    km.textGenerationFlags=data.textGenerationFlags;

    //    km. hJustification=data.hJustification;
    //    km.vJustification=data.vJustification;
    //    km.text=QString::fromStdString(data.text);
    //    km.style=QString::fromStdString(data.style);
    //    km.angle=data.angle;

    //    list_Text->insert(index,km);
    //    index++;

}

void M_dxfIO::addArcAlignedText(const DL_ArcAlignedTextData &data)
{

    //    QMutexLocker locker(&m_mutex);
    //    Data_ArcAlignedText km;

    //    km.text=QString::fromStdString(data.text);
    //    km.font=QString::fromStdString(data.font);
    //    km.style=QString::fromStdString(data.style);

    //    km.cx=data.cx;
    //    km.cy=data.cy;
    //    km.cz=data.cz;
    //    km.radius=data.radius;


    //    km.xScaleFactor=data.xScaleFactor;
    //    km.height=data.height;
    //    km.spacing=data.spacing;
    //    km.offset=data.offset;
    //    km.rightOffset=data.rightOffset;
    //    km.leftOffset=data.leftOffset;
    //    km.startAngle=data.startAngle;
    //    km.endAngle=data.endAngle;
    //    km.reversedCharacterOrder=data.reversedCharacterOrder;
    //    km.direction=data.direction;
    //    km.alignment=data.alignment;
    //    km.side=data.side;
    //    km.bold=data.bold;
    //    km.italic=data.italic;
    //    km.underline=data.underline;
    //    km.characerSet=data.characerSet;
    //    km.pitch=data.pitch;

    //    km.shxFont=data.shxFont;
    //    km.wizard=data.wizard;
    //    km.arcHandle=data.arcHandle;

    //    list_ArcAlignedText->insert(index,km);
    //    index++;
}

