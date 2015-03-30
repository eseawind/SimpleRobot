#ifndef STLPARSER_H
#define STLPARSER_H

#include <QVector>
#include <QString>
typedef struct STLTriangle {
    float normalf[3];
    float vectexf[3][3];
    short attribute;
} STLTriangle;

struct STLObject {
   char head[80];
   unsigned int facesi;
   QVector<STLTriangle> triangles;
};

class STLParser
{
public:
    STLParser();
    ~STLParser();
    STLObject *getSTLObjectFromBinary(QString filename);
    STLObject *getSTLObjectFromAscii(QString filename);
    void destroySTLObject(STLObject *obj);
    bool confirmRule(STLObject *obj);
};

#endif // STLPARSER_H
