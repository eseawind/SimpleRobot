#include "stlparser.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <assert.h>
STLParser::STLParser()
{

}

STLParser::~STLParser()
{

}

STLObject* STLParser::getSTLObjectFromBinary(const QString &filename)
{
    char buff[84];
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"open failed!";
        return NULL;
    }
    if(file.read(buff, 84)!=84){
        qDebug()<<"read head failed!";
        file.close();
        return NULL;
    }
    STLObject *obj = new STLObject;
    memcpy((void*)obj->head, buff, 80);
    memcpy((void*)&obj->facesi, &buff[80], 4);

    obj->triangles.resize(obj->facesi);
    for(unsigned int i=0; i<obj->facesi; ++i){
        for(int j=0; j<3; ++j){
            assert(4==file.read((char*)&obj->triangles[i].normalf[j], 4));
        }
        for(int j=0; j<3; ++j){
            for(int k=0; k<3; ++k){
                assert(4==file.read((char*)&obj->triangles[i].vectexf[j][k], 4));
            }
        }
        assert(2==file.read((char*)&obj->triangles[i].attribute, 2));
    }
    assert(file.atEnd());
    //qDebug()<<"faces: "<<obj->facesi;
    confirmRule(obj);
    return obj;
}

STLObject *STLParser::getSTLObjectFromAscii(const QString &filename)
{
    char head[5];
    char line[256];
    QFile file(filename);
    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"open failed!";
        return NULL;
    }
    if(file.read(head, 5)!=5&&strncmp(head,"soild", 5)!=0){
        qDebug()<<"read head failed!";
        file.close();
        return NULL;
    }
    STLObject *obj = new STLObject;
    STLTriangle triangle;
    int vertexIndex = 0;
    obj->facesi = 0;
    while(file.readLine(line, 256)&&!file.atEnd()){
        if(strstr(line,  "facet normal")){
            QString normal(line);
            QStringList l_normal = normal.split(" ", QString::SkipEmptyParts);
            triangle.normalf[0] = l_normal[2].toFloat();
            triangle.normalf[1] = l_normal[3].toFloat();
            triangle.normalf[2] = l_normal[4].toFloat();
        }
        else if(strstr(line, "outer loop")){
            vertexIndex = 0;
        }
        else if(strstr(line, "vertex")){
            QString vertex(line);
            QStringList l_vertex = vertex.split(" ", QString::SkipEmptyParts);
            triangle.vectexf[vertexIndex][0] = l_vertex[1].toFloat();
            triangle.vectexf[vertexIndex][1] = l_vertex[2].toFloat();
            triangle.vectexf[vertexIndex][2] = l_vertex[3].toFloat();
            ++vertexIndex;
        }
        else if(strstr(line, "endfacet")){
            obj->triangles.push_back(triangle);
            obj->facesi++;
        }

    }
    //qDebug()<<"faces: "<<obj->facesi;
    //confirmRule(obj);
    return obj;
}

bool STLParser::confirmRule(STLObject *obj)
{
    if(!obj) return false;
//    for(int i=0; i<12; ++i){
//        qDebug()<<"normal:"<<obj->triangles[i].normalf[0]
//               <<obj->triangles[i].normalf[1]
//              <<obj->triangles[i].normalf[2];
//        for(int j=0; j<3; ++j){
//        qDebug()<<i<<": "<<obj->triangles[i].vectexf[j][0]
//               <<obj->triangles[i].vectexf[j][1]
//              <<obj->triangles[i].vectexf[j][2];
//        }
//    }
    return true;
}

void destroySTLObject(STLObject *obj)
{
    if(obj)
        delete obj;
}
