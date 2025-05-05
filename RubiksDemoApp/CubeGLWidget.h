#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QElapsedTimer>
#include <QMatrix4x4>

class CubeGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    explicit CubeGLWidget(QWidget* parent = nullptr);
    ~CubeGLWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer            m_vbo{ QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer            m_ebo{ QOpenGLBuffer::IndexBuffer };
    QOpenGLShaderProgram     m_program;
    QElapsedTimer            m_timer;
};
