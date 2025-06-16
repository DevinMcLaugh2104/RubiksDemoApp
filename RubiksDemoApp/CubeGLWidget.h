#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QTimer>
#include <QMouseEvent>
#include "CubeCore.h"

class CubeGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    explicit CubeGLWidget(QWidget* parent = nullptr);
    ~CubeGLWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer            m_vbo{ QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer            m_ebo{ QOpenGLBuffer::IndexBuffer };
    QOpenGLShaderProgram     m_program;
    QElapsedTimer            m_timer;

    QMatrix4x4               m_projection;
    QMatrix4x4               m_view;
    float                    m_xRot = 25.0f;
    float                    m_yRot = 30.0f;
    QPoint                   m_lastPos;

    CubeCore                 m_cube;

    void drawCubie(const Cubie& cubie);
};

