#ifndef STLPARSER_H
#define STLPARSER_H

#include <QVector>
#include <QString>
/// STL三角面类
typedef struct STLTriangle {
    float normalf[3];
    float vectexf[3][3];
    short attribute;
} STLTriangle;
/// STL数据类
struct STLObject {
   char head[80];
   unsigned int facesi;
   QVector<STLTriangle> triangles;
};
/// STL解析器
class STLParser
{
public:
    /// A constructor
    STLParser();
    /// A destructor
    ~STLParser();
    /// 从二进制文件获取STL数据
    /// @param filename 文件名
    /// @return STLObject指针
    STLObject *getSTLObjectFromBinary(const QString &filename);
    /// 从ASCII文件获取STL数据
    /// @param filename 文件名
    /// @return STLObject指针
    STLObject *getSTLObjectFromAscii(const QString &filename);
    /// 销毁STLObject
    /// @param obj 从getSTLObjectFrom*获取到的指针
    void destroySTLObject(STLObject *obj);
    /// @todo 检测STL文件是否满足格式要求
    bool confirmRule(STLObject *obj);
};

#endif // STLPARSER_H
