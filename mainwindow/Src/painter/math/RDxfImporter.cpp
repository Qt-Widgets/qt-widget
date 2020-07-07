
#include <QDir>
#include <QList>
#include <QFileInfo>
#include <QTextCodec>
#include <QMultiMap>
#include <QString>
#include  <QDebug>

#include "RCircle.h"
#include "REllipse.h"
#include "RArc.h"
#include "RRay.h"
#include "RXLine.h"
#include "RLine.h"
#include "RPoint.h"

#include "RColor.h"
#include "RDxfServices.h"
#include "RLineweight.h"

#include "Src/dxflib/dl_dxf.h"

#include "Src/dxflib/m_dxfio.h"

#include "RDxfImporter.h"
#include "RVector.h"


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



RDxfImporter::RDxfImporter():polylinePlineGen(false),leaderArrowHead(false),inDict(false) {
}

RDxfImporter::~RDxfImporter() {
}

bool RDxfImporter::importFile(const QString& fileName) {


    this->fileName = fileName;
    QFileInfo fi(fileName);
    if (!fi.exists()) {
        qWarning() << "RDxfImporter::importFile: "
                   << "file does not exist: " << fileName;
        return false;
    }

    if (fi.size()==0) {
        qWarning() << "RDxfImporter::importFile: "
                   << "file has 0 length: " << fileName;
        return false;
    }

    // detect QCAD 2 DXF file format (dxflib 2.x):


    lockedLayers.clear();
    mtext = "";


    DL_Dxf dxflib;

    bool success = dxflib.in(fileName.toStdWString(), this);

    if (success==false) {
        qWarning() << "Cannot open DXF file: " << fileName;
        return false;
    }


    // lock locked layers now. they are unlocked during import to load
    // the entities on them:
    //    for (int i=0; i<lockedLayers.size(); i++) {
    //        QSharedPointer<RLayer> layer = document->queryLayer(lockedLayers.at(i));

    //    }


    return true;
}

void RDxfImporter::processCodeValuePair(unsigned int groupCode, const std::string& groupValue) {
    Q_UNUSED(groupCode)
    Q_UNUSED(groupValue)

    //printf("group code: %d\n", groupCode);
    //printf("group value: %s\n", groupValue);
}




void RDxfImporter::addLayer(const DL_LayerData& data) {
    QString layerName = QString::fromStdString(data.name);

    bool off = attributes.getColor()<0;
    bool frozen = data.flags&0x01;
    bool locked = data.flags&0x04;
    attributes.setColor(abs(attributes.getColor()));
    RColor color = RDxfServices::attributesToColor(
        attributes.getColor(), attributes.getColor24(), dxfColors, true);

    RLineweight::Lineweight lw = RDxfServices::numberToWeight(attributes.getWidth());

    LayerInfo layer(ptrScene);
    layer.name=layerName;
    if(color.toCompat()==Qt::white){
        color=Qt::black;
    }
    layer.setColor(color);
    layer.setVisible(!off);
    layer.setisEnabled(!locked);

    ptrScene->addLayer(layer);

}

void RDxfImporter::addLinetype(const DL_LinetypeData& data) {

}

void RDxfImporter::addLinetypeDash(double length) {

}

void RDxfImporter::addBlock(const DL_BlockData& data) {


}

void RDxfImporter::endBlock() {

}

void RDxfImporter::endEntity() {
    if (polyline.countVertices()>1) {

        ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RPolyline(polyline))));

        polyline = RPolyline();
    }

    if (spline.isValid()) {
        //        bool periodic = true;
        //        QList<RVector> controlPoints = spline.getControlPoints();
        //        QList<double> knots = spline.getKnotVector();

        //        for (int i=0; i<spline.getDegree(); i++) {
        //            if (controlPoints.at(i).getDistanceTo(controlPoints.at(controlPoints.size()-spline.getDegree()+i)) > RS::PointTolerance) {
        //                periodic = false;
        //            }
        //        }

        //        if (periodic) {
        //            for (int i=0; i<spline.getDegree(); ++i) {
        //                controlPoints.removeLast();
        //            }
        //            spline.setControlPoints(controlPoints);
        //            spline.setPeriodic(true);
        //        }
        //        else {
        //            spline.setControlPoints(controlPoints);
        //            spline.setKnotVector(knots);
        //        }

        QList<RVector> fitPoints = spline.getFitPoints();
        if (!fitPoints.isEmpty() && spline.isPeriodic()) {
            // TODO: check for start / end tangent equality if given:
            if (fitPoints.first().equalsFuzzy(fitPoints.last())) {
                spline.setPeriodic(true);
                //fitPoints.removeLast();
                spline.removeLastFitPoint();
            }
            else {
                spline.setPeriodic(false);
            }
        }
        else {
            spline.setPeriodic(false);
        }

        QList<double> kv = spline.getKnotVector();

        if (kv.size()>=2) {
            // remove historical, superfluous knots:
            kv.removeFirst();
            kv.removeLast();
        }

        spline.setKnotVector(kv);

        ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RSpline(spline))));



        spline = RSpline();
    }

}

void RDxfImporter::endSection() {
    xData.clear();
    xDataAppId = "";
}


void RDxfImporter::addPoint(const DL_PointData& data) {
    RVector v(data.x, data.y);

     ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RPoint(v))));

}

void RDxfImporter::addLine(const DL_LineData& data) {
    RVector v1(data.x1, data.y1);
    RVector v2(data.x2, data.y2);

     ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RLine(v1, v2))));

}

void RDxfImporter::addXLine(const DL_XLineData& data) {
    RVector v1(data.bx, data.by);
    RVector v2(data.dx, data.dy);

        ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RXLine(v1, v2))));

}

void RDxfImporter::addRay(const DL_RayData& data) {
    RVector v1(data.bx, data.by);
    RVector v2(data.dx, data.dy);


    ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RRay(v1, v2))));

}

void RDxfImporter::addArc(const DL_ArcData& data) {
    RVector v(data.cx, data.cy);

    ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RArc(v, data.radius,
                                                                                                                              RMath::deg2rad(data.angle1),
                                                                                                                              RMath::deg2rad(data.angle2),
                                                                                                                              false))));

}

void RDxfImporter::addEllipse(const DL_EllipseData& data) {
    RVector v1(data.cx, data.cy);
    RVector v2(data.mx, data.my);

    ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new REllipse(v1,v2, data.ratio,
                                                                                                                                  data.angle1,
                                                                                                                                  data.angle2,
                                                                                                                                  false))));


}

void RDxfImporter::addCircle(const DL_CircleData& data) {
    RVector v(data.cx, data.cy);

    ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RCircle(v, data.radius))));

}

void RDxfImporter::addPolyline(const DL_PolylineData& data) {
    polyline = RPolyline();
    polyline.setClosed(data.flags&0x1);
    polylinePlineGen = data.flags&0x80;
}

void RDxfImporter::addVertex(const DL_VertexData& data) {
    RVector v(data.x, data.y);
    polyline.appendVertex(v, data.bulge);
}

void RDxfImporter::addSpline(const DL_SplineData& data) {
    spline.setDegree(data.degree);
    spline.setPeriodic(data.flags&0x2);
    RVector tanS(data.tangentStartX, data.tangentStartY, data.tangentStartZ);
    RVector tanE(data.tangentEndX, data.tangentEndY, data.tangentEndZ);
    if (tanS.getMagnitude()>RS::PointTolerance) {
        spline.setTangentAtStart(tanS);
    }
    if (tanE.getMagnitude()>RS::PointTolerance) {
        spline.setTangentAtEnd(tanE);
    }
}

void RDxfImporter::addControlPoint(const DL_ControlPointData& data) {
    RVector v(data.x, data.y);

    // weight is ignored for now:
    //double w = data.w;

    spline.appendControlPoint(v);
}

void RDxfImporter::addFitPoint(const DL_FitPointData& data) {
    RVector v(data.x, data.y);
    spline.appendFitPoint(v);
}

void RDxfImporter::addKnot(const DL_KnotData& data) {
    //    printf("add knot\n");
    spline.appendKnot(data.k);
}

void RDxfImporter::addInsert(const DL_InsertData& data) {

}

void RDxfImporter::addSolid(const DL_SolidData& data) {

    RVector v1(data.x[0], data.y[0], data.z[0]);
    RVector v2(data.x[1], data.y[1], data.z[1]);
    RVector v3(data.x[2], data.y[2], data.z[2]);
    RVector v4(data.x[3], data.y[3], data.z[3]);

    RPolyline d;
    if (v3.equalsFuzzy(v4)) {
        // last two vertices identical: triangle:
        d.appendVertex(v1);
        d.appendVertex(v2);
        d.appendVertex(v3);
        d.setClosed(true);

    }
    else {

        d.appendVertex(v1);
        d.appendVertex(v2);
        d.appendVertex(v3);
        d.appendVertex(v4);
        d.setClosed(true);
    }

     ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RPolyline(d))));

}

void RDxfImporter::addTrace(const DL_TraceData& data) {

    RVector v1(data.x[0], data.y[0], data.z[0]);
    RVector v2(data.x[1], data.y[1], data.z[1]);
    RVector v3(data.x[2], data.y[2], data.z[2]);
    RVector v4(data.x[3], data.y[3], data.z[3]);

    RPolyline poly;

    poly.appendVertex(v1);
    poly.appendVertex(v2);
    poly.appendVertex(v3);
    poly.appendVertex(v4);
    poly.setClosed(true);

    ptrScene->addHItem(QString::fromStdString(attributes.getLayer()),new HGraphicsPathItem(QSharedPointer<RShape>(new RPolyline(poly))));

}

void RDxfImporter::addTextStyle(const DL_StyleData& data) {
    //    QString xDataFont = getXDataString("ACAD", 1000, 0);
    //    int xDataFlags = getXDataInt("ACAD", 1071, 0);

    //    RDxfTextStyle s;

    //    s.font = decode(data.primaryFontFile.c_str());
    //    //qDebug() << "text style: name:" << (const char*)data.name.c_str();
    //    //qDebug() << "text style: s.font:" << s.font;
    //    if (s.font.isEmpty()) {
    //        s.font = xDataFont;
    //        //qDebug() << "text style: xDataFont:" << xDataFont;
    //    }

    //    s.italic = xDataFlags&0x1000000;
    //    s.bold = xDataFlags&0x2000000;

    //    textStyles.insert(decode(data.name.c_str()), s);
}

void RDxfImporter::addMTextChunk(const std::string& text) {
    mtext.append(text.c_str());
}

void RDxfImporter::addMText(const DL_MTextData& data) {
    RVector ip(data.ipx, data.ipy);
    //RVector dirx(data.dirx, data.diry);
    RS::VAlign valign;
    RS::HAlign halign;
    RS::TextDrawingDirection dir;
    RS::TextLineSpacingStyle lss;

    //    RDxfTextStyle s = textStyles.value(decode(data.style.c_str()), RDxfTextStyle());

    //    // QCAD 2 compat: use style name as font name:
    //    if (s.font.isEmpty()) {
    //        s.font = decode(data.style.c_str());
    //    }

    //    if (data.attachmentPoint<=3) {
    //        valign=RS::VAlignTop;
    //    } else if (data.attachmentPoint<=6) {
    //        valign=RS::VAlignMiddle;
    //    } else {
    //        valign=RS::VAlignBottom;
    //    }

    //    if (data.attachmentPoint%3==1) {
    //        halign=RS::HAlignLeft;
    //    } else if (data.attachmentPoint%3==2) {
    //        halign=RS::HAlignCenter;
    //    } else {
    //        halign=RS::HAlignRight;
    //    }

    //    if (data.drawingDirection==1) {
    //        dir = RS::LeftToRight;
    //    } else if (data.drawingDirection==3) {
    //        dir = RS::TopToBottom;
    //    } else {
    //        dir = RS::ByStyle;
    //    }

    //    if (data.lineSpacingStyle==1) {
    //        lss = RS::AtLeast;
    //    } else {
    //        lss = RS::Exact;
    //    }

    //    mtext.append(data.text.c_str());
    //    mtext.replace(QByteArray("^ "), QByteArray("^"));
    //    QString mtextString = QString(mtext);

    //    QVariant vDwgCodePage = document->getKnownVariable(RS::DWGCODEPAGE);
    //    if (vDwgCodePage.isValid()) {
    //        QString dwgCodePage = vDwgCodePage.toString();
    //        QString enc = getEncoding(dwgCodePage);

    //        // get the text codec:
    //        QTextCodec* codec = QTextCodec::codecForName(enc.toLatin1());
    //        if (codec!=NULL) {
    //            mtextString = codec->toUnicode(mtext);
    //        }
    //        else {
    //            qWarning() << "RDxfImporter::addMText: unsupported text codec: " << enc;
    //        }
    //    }

    //    // use default style for the drawing:
    //    if (s.font.isEmpty()) {
    //        // japanese, cyrillic:
    //        QString codepage = document->getKnownVariable(RS::DWGCODEPAGE, "ANSI_1252").toString().toUpper();
    //        if (codepage=="ANSI_932" || codepage=="ANSI_1251") {
    //            s.font = "Unicode";
    //        } else {
    //            s.font = document->getKnownVariable(RS::TEXTSTYLE, "Standard").toString();
    //        }
    //    }

    //    dxfServices.fixVersion2String(mtextString);

    //    RTextData d(
    //                RVector::invalid, ip,
    //                data.height, data.width,
    //                valign, halign,
    //                dir, lss,
    //                data.lineSpacingFactor,
    //                mtextString, s.font,
    //                s.bold,
    //                s.italic,
    //                data.angle,
    //                false
    //                );

    //    QSharedPointer<RTextEntity> entity(new RTextEntity(document, d));
    //    importEntity(entity);

    //    mtext = "";
}

void RDxfImporter::getTextBasedData(const DL_TextData &data)
{

}


/**
 * Implementation of the method which handles
 * texts (TEXT).
 */
void RDxfImporter::addText(const DL_TextData& data) {

}

void RDxfImporter::addArcAlignedText(const DL_ArcAlignedTextData& data) {
    // TODO
    qDebug() << "addArcAlignedText";
    qDebug() << "text" << data.text.c_str();
    qDebug() << "cx" << data.cx;
    qDebug() << "cy" << data.cy;
    qDebug() << "cz" << data.cz;
    qDebug() << "radius" << data.radius;
    qDebug() << "font" << data.font.c_str();
    qDebug() << "style" << data.style.c_str();
    qDebug() << "alignment" << data.alignment;
    qDebug() << "char set" << data.characerSet;
    qDebug() << "char order" << data.reversedCharacterOrder;
}


/**
 * Implementation of the method which handles
 * block attributes (ATTRIB).
 */
void RDxfImporter::addAttribute(const DL_AttributeData& data) {

}


void RDxfImporter::convDimensionData(const DL_DimensionData& data) {

}

void RDxfImporter::addDimAlign(const DL_DimensionData& data, const DL_DimAlignedData& edata) {

}

void RDxfImporter::addDimLinear(const DL_DimensionData& data,
                                const DL_DimLinearData& edata) {

}

void RDxfImporter::addDimRadial(const DL_DimensionData& data,
                                const DL_DimRadialData& edata) {

}

void RDxfImporter::addDimDiametric(const DL_DimensionData& data,
                                   const DL_DimDiametricData& edata) {

}

void RDxfImporter::addDimAngular(const DL_DimensionData& data,
                                 const DL_DimAngularData& edata) {

}

void RDxfImporter::addDimAngular3P(const DL_DimensionData& data,
                                   const DL_DimAngular3PData& edata) {


}

void RDxfImporter::addDimOrdinate(const DL_DimensionData& data,
                                  const DL_DimOrdinateData& edata) {

}

void RDxfImporter::addLeader(const DL_LeaderData& data) {

}

void RDxfImporter::addLeaderVertex(const DL_LeaderVertexData& data) {

}

void RDxfImporter::addHatch(const DL_HatchData& data) {

}

void RDxfImporter::addHatchLoop(const DL_HatchLoopData& data) {
    Q_UNUSED(data)

}

void RDxfImporter::addHatchEdge(const DL_HatchEdgeData& data) {

}

void RDxfImporter::addImage(const DL_ImageData& data) {

}

void RDxfImporter::linkImage(const DL_ImageDefData& data) {

}

void RDxfImporter::addXRecord(const std::string& handle) {
    if (qcadDict.count(handle.c_str())==1) {
        variableKey = qcadDict[handle.c_str()];
    }
    else {
        variableKey = QString();
    }
}

void RDxfImporter::addXRecordString(int code, const std::string& value) {
    Q_UNUSED(code)

}

void RDxfImporter::addXRecordReal(int code, double value) {
    Q_UNUSED(code)


}

void RDxfImporter::addXRecordInt(int code, int value) {
    Q_UNUSED(code)


}

void RDxfImporter::addXRecordBool(int code, bool value) {
    Q_UNUSED(code)


}

void RDxfImporter::addXDataApp(const std::string& appId) {

}

void RDxfImporter::addXDataString(int code, const std::string& value) {
    if (!xData.contains(xDataAppId)) {
        //qWarning() << "RDxfImporter::addXDataString: app ID not found: " << xDataAppId;
        return;
    }


}

void RDxfImporter::addXDataReal(int code, double value) {
    if (!xData.contains(xDataAppId)) {
        //qWarning() << "RDxfImporter::addXDataString: app ID not found: " << xDataAppId;
        return;
    }

    xData[xDataAppId].append(QPair<int, QVariant>(code, value));
}

void RDxfImporter::addXDataInt(int code, int value) {
    if (!xData.contains(xDataAppId)) {
        //qWarning() << "RDxfImporter::addXDataString: app ID not found: " << xDataAppId;
        return;
    }

    xData[xDataAppId].append(QPair<int, QVariant>(code, value));
}

void RDxfImporter::addDictionary(const DL_DictionaryData& data) {
    if (qcadDictHandle==data.handle.c_str()) {
        inDict = true;
    }
}

void RDxfImporter::addDictionaryEntry(const DL_DictionaryEntryData& data) {
    if (data.name=="QCAD_OBJECTS") {
        qcadDictHandle = data.handle.c_str();
        return;
    }

    if (inDict) {
        qcadDict[data.handle.c_str()] = data.name.c_str();
    }
}

void RDxfImporter::setVariableVector(const std::string& key,
                                     double v1, double v2, double v3, int code) {
    Q_UNUSED(code)


}

void RDxfImporter::setVariableString(const std::string& key,
                                     const std::string& value, int code) {
    Q_UNUSED(code)


}

void RDxfImporter::setVariableInt(const std::string& key, int value, int code) {
    Q_UNUSED(code)


}

void RDxfImporter::setVariableDouble(const std::string& key, double value, int code) {
    Q_UNUSED(code)


}

/**
 * Tries to convert the given DXF encoding string to an encoding recognized by Qt.
 */
QString RDxfImporter::getEncoding(const QString& str) {
    QString l=str.toLower();

    if (l=="latin1" || l=="ansi_1252" || l=="iso-8859-1" ||
            l=="cp819" || l=="csiso" || l=="ibm819" || l=="iso_8859-1" ||
            l=="iso8859-1" || l=="iso-ir-100" || l=="l1") {
        return "Latin1";
    } else if (l=="big5" || l=="ansi_950" || l=="cn-big5" || l=="csbig5" ||
               l=="x-x-big5") {
        return "Big5";
    } else if (l=="big5-hkscs") {
        return "Big5-HKSCS";
    } else if (l=="eucjp" || l=="euc-jp" || l=="cseucpkdfmtjapanese" ||
               l=="x-euc" || l=="x-euc-jp") {
        return "eucJP";
    } else if (l=="euckr") {
        return "eucKR";
    } else if (l=="gb2312" || l=="gb2312" || l=="chinese" || l=="cn-gb" ||
               l=="csgb2312" || l=="csgb231280" || l=="csiso58gb231280" ||
               l=="gb_2312-80" || l=="gb231280" || l=="gb2312-80" || l=="gbk" ||
               l=="iso-ir-58") {
        return "GB2312";
    } else if (l=="gbk") {
        return "GBK";
    } else if (l=="gb18030") {
        return "GB18030";
    } else if (l=="jis7") {
        return "JIS7";
    } else if (l=="shift-jis" || l=="ansi_932" || l=="shift_jis" || l=="csShiftJIS" ||
               l=="cswindows31j" || l=="ms_kanji" || l=="x-ms-cp932" || l=="x-sjis") {
        return "Shift-JIS";
    } else if (l=="tscii") {
        return "TSCII";
    } else if (l=="utf88-bit") {
        return "utf88-bit";
    } else if (l=="utf16") {
        return "utf16";
    } else if (l=="koi8-r") {
        return "KOI8-R";
    } else if (l=="koi8-u") {
        return "KOI8-U";
    } else if (l=="iso8859-1") {
        return "ISO8859-1";
    } else if (l=="iso8859-2") {
        return "ISO8859-2";
    } else if (l=="iso8859-3") {
        return "ISO8859-3";
    } else if (l=="iso8859-4" || l=="ansi_1257") {
        return "ISO8859-4";
    } else if (l=="iso8859-5") {
        return "ISO8859-5";
    } else if (l=="iso8859-6" || l=="ansi_1256") {
        return "ISO8859-6";
    } else if (l=="iso8859-7" || l=="ansi_1253") {
        return "ISO8859-7";
    } else if (l=="iso8859-8") {
        return "ISO8859-8";
    } else if (l=="iso8859-8-i" || l=="ansi_1255") {
        return "ISO8859-8-i";
    } else if (l=="iso8859-9" || l=="ansi_1254") {
        return "ISO8859-9";
    } else if (l=="iso8859-10") {
        return "ISO8859-10";
    } else if (l=="iso8859-13") {
        return "ISO8859-13";
    } else if (l=="iso8859-14") {
        return "ISO8859-14";
    } else if (l=="iso8859-15") {
        return "ISO8859-15";
    } else if (l=="ibm 850") {
        return "IBM 850";
    } else if (l=="ibm 866") {
        return "IBM 866";
    } else if (l=="cp874") {
        return "CP874";
    } else if (l=="cp1250") {
        return "CP1250";
    } else if (l=="cp1251") {
        return "CP1251";
    } else if (l=="cp1252") {
        return "CP1252";
    } else if (l=="cp1253") {
        return "CP1253";
    } else if (l=="cp1254") {
        return "CP1254";
    } else if (l=="cp1255") {
        return "CP1255";
    } else if (l=="cp1256") {
        return "CP1256";
    } else if (l=="cp1257") {
        return "CP1257";
    } else if (l=="cp1258") {
        return "CP1258";
    } else if (l=="apple roman") {
        return "Apple Roman";
    } else if (l=="tis-620") {
        return "TIS-620";
    }

    return "Latin1";
}

QString RDxfImporter::getXDataString(const QString& appId, int code, int pos) {
    if (!xData.contains(appId)) {
        return QString::null;
    }

    int c = 0;
    for (int i=0; i<xData[appId].count(); i++) {
        if (c==pos && xData[appId][i].first==code) {
            return xData[appId][i].second.toString();
        }
    }

    return QString::null;
}

int RDxfImporter::getXDataInt(const QString& appId, int code, int pos) {
    if (!xData.contains(appId)) {
        return 0;
    }

    int c = 0;
    for (int i=0; i<xData[appId].count(); i++) {
        if (c==pos && xData[appId][i].first==code) {
            return xData[appId][i].second.toInt();
        }
    }

    return 0;
}


