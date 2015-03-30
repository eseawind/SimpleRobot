#include "model.h"
#include "stlparser.h"
#include <assert.h>
Model::Model():
    speedFactor(0.3),
    defaultSpeed(100)
{

    STLParser parser;
    object[BASE] = parser.getSTLObjectFromBinary(":/base.stl");
    object[MASTER] = parser.getSTLObjectFromBinary(":/master.stl");
    object[ASSISTANT] = parser.getSTLObjectFromBinary(":/assistant.stl");
    object[BAR] = parser.getSTLObjectFromBinary(":/bar.stl");

    assert(object[BASE]&&object[MASTER]&&object[ASSISTANT]&&object[BAR]);

    for(int i=0; i<4; ++i){
        getVerteces(object[i], arrayV[i], arrayN[i]);
        verteces += arrayV[i];
        normals += arrayN[i];
    }

    for(int i=0; i<4; ++i){
        data.cur[i] = data.target[i] = 0;
        data.speed[i] = defaultSpeed;
        data.directionFactor[i] = 1;
        data.direction[i] = 0;
    }

    center_base.setToIdentity();
    center_base.translate(-100, 0, -75);
    matrix_base.setToIdentity();

    center_master.setToIdentity();
    center_master.translate(-275, 0, -65);
    matrix_master.setToIdentity();
    matrix_master.translate(100,258,75);

    center_assistant.setToIdentity();
    center_assistant.translate(-276, -14, -50);
    matrix_assistant.setToIdentity();
    matrix_assistant.translate(50, 34, 65);

    center_bar.setToIdentity();
    center_bar.translate(-6.5, 0, -20);
    matrix_bar.setToIdentity();
    matrix_bar.translate(50, -10, 50);

}

Model::~Model()
{

}

const QVector<float> &Model::getModelVerteces() const
{
   return verteces;
}

const QVector<float> &Model::getModelNormals() const
{
   return normals;
}

void Model::getVerteces(STLObject *obj, QVector<float> &vertecs, QVector<float> &normals)
{
    for(unsigned int i=0; i<obj->facesi; ++i){
        for(int j=0; j<3; ++j)
            for(int k=0; k<3; ++k){
                vertecs.push_back(obj->triangles[i].vectexf[j][k]);
                normals.push_back(obj->triangles[i].normalf[k]);
            }
    }
}
