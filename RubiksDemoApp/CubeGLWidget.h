#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMouseEvent>
#include "RubiksCube.h"            // <<< NEW

class CubeGLWidget : public QOpenGLWidget,
    protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CubeGLWidget(QWidget* parent = nullptr);
    ~CubeGLWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    void rotateCube(int dx, int dy);
    bool intersectsCube(const QVector3D& O, const QVector3D& D);

    // GL objects
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer            m_vbo{ QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer            m_ebo{ QOpenGLBuffer::IndexBuffer };
    QOpenGLShaderProgram     m_prog;

    // camera
    QMatrix4x4 m_proj, m_view;
    float      m_xRot = 25.f, m_yRot = 30.f;
    QPoint     m_lastPos;
    bool       m_rotating = false;
    QVector3D  m_camPos{ 0,0,10 };

    // picking helpers
    QMatrix4x4 m_pickProj, m_pickView;

    // cube data
    RubiksCube m_cube;             // <<< NEW
};
