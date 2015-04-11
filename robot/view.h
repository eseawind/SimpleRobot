#ifndef VIEW_H
#define VIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Model;
/// 视图类
class View : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    /// A construtor
    View(Model *model, QWidget *parent=0);
    /// A destructor
    ~View();
    /// 旋转视图
    /// @param thetaX 绕x轴角度
    /// @param thetaY 绕y轴角度
    /// @param thetaZ 绕z轴角度
    void rotateView(float thetaX, float thetaY, float thetaZ) {
       theta[0] += thetaX;
       theta[1] += thetaY;
       theta[2] += thetaZ;
       update();
    }

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void initVBO(QOpenGLBuffer &vbo, const QVector<GLfloat> &data);
    QMatrix4x4 paintBase(QMatrix4x4 &mvMatrix);
    QMatrix4x4 paintMaster(QMatrix4x4 &mvMatrix);
    QMatrix4x4 paintAssistant(QMatrix4x4 &mvMatrix);
    QMatrix4x4 paintBar(QMatrix4x4 &mvMatrix);

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo_verteces, m_vbo_normals;
    QOpenGLShaderProgram m_program;
    QMatrix4x4 m_camera, m_world, m_projection;
    Model *model;



    float theta[3];
};

#endif // VIEW_H
