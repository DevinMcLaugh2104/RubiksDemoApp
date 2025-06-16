#include "CubeGLWidget.h"
#include <QOpenGLShader>
#include <QMouseEvent>
#include <iostream>

// Vertex shader
static const char* vertexShaderSource = R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    vertexColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

// Fragment shader
static const char* fragmentShaderSource = R"(#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vertexColor, 1.0);
})";

CubeGLWidget::CubeGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

CubeGLWidget::~CubeGLWidget() {
    makeCurrent();
    m_ebo.destroy();
    m_vbo.destroy();
    m_vao.destroy();
    m_program.removeAllShaders();
    doneCurrent();
}

void CubeGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // Compile and link shaders
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource))
        qWarning("Vertex shader error:\n%s", m_program.log().toLocal8Bit().constData());
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource))
        qWarning("Fragment shader error:\n%s", m_program.log().toLocal8Bit().constData());

    m_program.bindAttributeLocation("aPos", 0);
    m_program.bindAttributeLocation("aColor", 1);

    if (!m_program.link()) {
        qWarning("Shader link error:\n%s", m_program.log().toLocal8Bit().constData());
    }

    m_program.bind();  // <<-- CRUCIAL

    static const float vertices[] = {
        // positions         // colors
        -0.5f,-0.5f,-0.5f,   1,0,0,
         0.5f,-0.5f,-0.5f,   0,1,0,
         0.5f, 0.5f,-0.5f,   0,0,1,
        -0.5f, 0.5f,-0.5f,   1,1,0,
        -0.5f,-0.5f, 0.5f,   1,0,1,
         0.5f,-0.5f, 0.5f,   0,1,1,
         0.5f, 0.5f, 0.5f,   1,1,1,
        -0.5f, 0.5f, 0.5f,   .5f,.5f,.5f
    };
    static const unsigned int indices[] = {
         0,1,2, 2,3,0,
         1,5,6, 6,2,1,
         5,4,7, 7,6,5,
         4,0,3, 3,7,4,
         3,2,6, 6,7,3,
         4,5,1, 1,0,4
    };

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(indices, sizeof(indices));

    m_program.enableAttributeArray(0); // location 0 -> aPos
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));

    m_program.enableAttributeArray(1); // location 1 -> aColor
    m_program.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

    m_vao.release();
    m_program.release();

    m_timer.start();
}


void CubeGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, float(w) / h, 0.1f, 100.0f);
}

void CubeGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program.bind();

    // Build view
    m_view.setToIdentity();
    m_view.translate(0, 0, -10);
    m_view.rotate(m_xRot, 1, 0, 0);
    m_view.rotate(m_yRot, 0, 1, 0);

    m_program.setUniformValue("view", m_view);
    m_program.setUniformValue("projection", m_projection);

    m_vao.bind();

    for (int x = 0; x < CubeCore::SIZE; ++x)
        for (int y = 0; y < CubeCore::SIZE; ++y)
            for (int z = 0; z < CubeCore::SIZE; ++z) {
                const auto& cubie = m_cube.getCubie(x, y, z);
                m_program.setUniformValue("model", cubie.transform);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            }

    m_vao.release();
    m_program.release();

    update();
}

void CubeGLWidget::mousePressEvent(QMouseEvent* event) {
    m_lastPos = event->pos();
}

void CubeGLWidget::mouseMoveEvent(QMouseEvent* event) {
    int dx = int(event->position().x()) - m_lastPos.x();
    int dy = int(event->position().y()) - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_xRot += dy;
        m_yRot += dx;
        update();
    }

    m_lastPos = event->position().toPoint(); 
}

void CubeGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    if ((event->position().toPoint() - m_lastPos).manhattanLength() > 20) {
        m_cube.rotateLayer('y', 1, true);
        update();
    }
}

