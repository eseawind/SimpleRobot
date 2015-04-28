#include "view.h"
#include "model.h"

View::View(Model *model, QWidget *parent) :
    QOpenGLWidget(parent)
{
    this->model = model;
    theta[0] = theta[1] = theta[2] = 0;
}

View::~View()
{
}

void View::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.6, 0.8, 0.9, 1);

    m_program.create();
    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/adsphong.vsh");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/adsphong.fsh");

    if(!m_program.link()){
        qDebug()<<m_program.log();
        exit(1);
    }

    m_program.bindAttributeLocation("vVertex", 0);
    m_program.bindAttributeLocation("vNormal", 1);
    m_program.bind();

    m_vao.create();
    m_vao.bind();

    initVBO(m_vbo_verteces, model->getModelVerteces());
    initVBO(m_vbo_normals, model->getModelNormals());

    m_vbo_verteces.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vbo_verteces.release();

    m_vbo_normals.bind();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vbo_normals.release();

    m_camera.setToIdentity();
    m_camera.translate(0,0,0);
    m_camera.rotate(-30,0,1,0);
    m_camera.rotate(20,1,0,0);
    m_camera.rotate(-10,0,0,1);

    m_world.setToIdentity();

    m_projection.ortho(-650, 650, -30, 680, -700, 650);
    //m_projection.perspective(45,1,-650, 650);
}
void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_world.setToIdentity();
    m_world.translate(-150, 300, 0);
    m_world.rotate(theta[0], 1, 0, 0);
    m_world.rotate(theta[1], 0, 1, 0);
    m_world.rotate(theta[2], 0, 0, 1);
    m_world.translate(150, -300, 0);

    QMatrix4x4 mvMatrix = m_camera * m_world;

    QVector3D vLightPosition(0, 0, 10000);

    QVector4D ambientColor(0.3,0.3,0.3,1);
    QVector4D diffuseColor(0.8,0.8,0.8,1);
    QVector4D specularColor(0.7,0.7,0.7,1);

    m_program.bind();
    m_vao.bind();

    m_program.setUniformValue("vLightPosition", vLightPosition);
    m_program.setUniformValue("ambientColor", ambientColor);
    m_program.setUniformValue("diffuseColor", diffuseColor);
    m_program.setUniformValue("specularColor", specularColor);

    m_vbo_verteces.bind();
    m_vbo_normals.bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    m_program.setUniformValue("diffuseColor", QVector4D(0.6,0.6,0.6,1));
    mvMatrix = paintBase(mvMatrix);
    m_program.setUniformValue("diffuseColor", diffuseColor);
    mvMatrix = paintMaster(mvMatrix);
    mvMatrix = paintAssistant(mvMatrix);
    mvMatrix = paintBar(mvMatrix);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    m_vbo_verteces.release();
    m_vbo_normals.release();
    m_program.release();
    m_vao.release();

}

void View::initVBO(QOpenGLBuffer &vbo, const QVector<GLfloat> &data)
{
    vbo.create();
    vbo.bind();
    vbo.allocate(data.constData(), data.size()*sizeof(GLfloat));
    vbo.release();
}

QMatrix4x4 View::paintBase(QMatrix4x4 &mv)
{
    static GLint first = 0;
    static GLint count = model->getModelVertecesNum(model->BASE);
    QMatrix4x4 mvMatrix = mv * model->center_base * model->matrix_base;
    QMatrix4x4 mvpMatrix = m_projection * mvMatrix;
    QMatrix3x3 normalMatrix;
    float *tdata = normalMatrix.data();
    float *sdata = mvMatrix.data();
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            tdata[i*3+j] = sdata[i*4+j];
        }

    m_program.setUniformValue("mvpMatrix", mvpMatrix);
    m_program.setUniformValue("mvMatrix", mvMatrix);
    m_program.setUniformValue("normalMatrix", normalMatrix);

    glDrawArrays(GL_TRIANGLES, first, count);

    return mvMatrix;
}

QMatrix4x4 View::paintMaster(QMatrix4x4 &mv)
{
    static GLint first = model->getModelVertecesNum(model->BASE);
    static GLint count = model->getModelVertecesNum(model->MASTER);
    QMatrix4x4 mvMatrix = mv * model->matrix_master * model->action_master * model->center_master;
    QMatrix4x4 mvpMatrix = m_projection * mvMatrix;
    QMatrix3x3 normalMatrix;
    float *tdata = normalMatrix.data();
    float *sdata = mvMatrix.data();
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            tdata[i*3+j] = sdata[i*4+j];
        }

    m_program.setUniformValue("mvpMatrix", mvpMatrix);
    m_program.setUniformValue("mvMatrix", mvMatrix);
    m_program.setUniformValue("normalMatrix", normalMatrix);

    glDrawArrays(GL_TRIANGLES, first, count);

    return mvMatrix;
}

QMatrix4x4 View::paintAssistant(QMatrix4x4 &mv)
{
    static GLint first = model->getModelVertecesNum(model->BASE)+model->getModelVertecesNum(model->MASTER);
    static GLint count = model->getModelVertecesNum(model->ASSISTANT);
    QMatrix4x4 mvMatrix = mv * model->matrix_assistant * model->action_assistant * model->center_assistant;
    QMatrix4x4 mvpMatrix = m_projection * mvMatrix;
    QMatrix3x3 normalMatrix;
    float *tdata = normalMatrix.data();
    float *sdata = mvMatrix.data();
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            tdata[i*3+j] = sdata[i*4+j];
        }

    m_program.setUniformValue("mvpMatrix", mvpMatrix);
    m_program.setUniformValue("mvMatrix", mvMatrix);
    m_program.setUniformValue("normalMatrix", normalMatrix);

    glDrawArrays(GL_TRIANGLES, first, count);

    return mvMatrix;
}

QMatrix4x4 View::paintBar(QMatrix4x4 &mv)
{
    static GLint first = model->getModelVertecesNum(model->BASE)+model->getModelVertecesNum(model->MASTER)
            +model->getModelVertecesNum(model->ASSISTANT);
    static GLint count = model->getModelVertecesNum(model->BAR);
    QMatrix4x4 mvMatrix = mv * model->matrix_bar * model->action_bar * model->action_bar_move * model->center_bar;
    QMatrix4x4 mvpMatrix = m_projection * mvMatrix;
    QMatrix3x3 normalMatrix;
    float *tdata = normalMatrix.data();
    float *sdata = mvMatrix.data();
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            tdata[i*3+j] = sdata[i*4+j];
        }

    m_program.setUniformValue("mvpMatrix", mvpMatrix);
    m_program.setUniformValue("mvMatrix", mvMatrix);
    m_program.setUniformValue("normalMatrix", normalMatrix);

    glDrawArrays(GL_TRIANGLES, first, count);

    return mvMatrix;
}
