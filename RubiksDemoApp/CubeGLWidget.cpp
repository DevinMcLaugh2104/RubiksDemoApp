#include "CubeGLWidget.h"
#include <QOpenGLShader>
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
}
)";

// Fragment shader
static const char* fragmentShaderSource = R"(#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

CubeGLWidget::CubeGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

CubeGLWidget::~CubeGLWidget() {
    // clean up Qt wrappers
    makeCurrent();
    m_ebo.destroy();
    m_vbo.destroy();
    m_vao.destroy();
    m_program.removeAllShaders();
    doneCurrent();
}

void CubeGLWidget::initializeGL() {
    // bind GL entry points
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // compile/link
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (!m_program.link()) {
        qWarning("Shader link error:\n%s", m_program.log().toLocal8Bit().constData());
    }

    // cube data
    static const float vertices[] = {
        // positions         // colors
        -0.5f,-0.5f,-0.5f,   1,0,0,
         0.5f,-0.5f,-0.5f,   0,1,0,
         0.5f, 0.5f,-0.5f,   0,0,1,
        -0.5f, 0.5f,-0.5f,   1,1,0,
        -0.5f,-0.5f, 0.5f,   1,0,1,
         0.5f,-0.5f, 0.5f,   0,1,1,
         0.5f, 0.5f, 0.5f,   1,1,1,
        -0.5f, 0.5f, 0.5f,  .5f,.5f,.5f
    };
    static const unsigned int indices[] = {
         0,1,2, 2,3,0,
         1,5,6, 6,2,1,
         5,4,7, 7,6,5,
         4,0,3, 3,7,4,
         3,2,6, 6,7,3,
         4,5,1, 1,0,4
    };

    // VAO/VBO/EBO setup
    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(indices, sizeof(indices));

    // map "aPos"
    m_program.enableAttributeArray("aPos");
    m_program.setAttributeBuffer("aPos", GL_FLOAT,
        /*offset=*/0, /*tupleSize=*/3,
        /*stride=*/6 * sizeof(float));
    // map "aColor"
    m_program.enableAttributeArray("aColor");
    m_program.setAttributeBuffer("aColor", GL_FLOAT,
        /*offset=*/3 * sizeof(float), /*tupleSize=*/3,
        /*stride=*/6 * sizeof(float));

    m_vao.release();

    // start timer for rotation
    m_timer.start();
}

void CubeGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void CubeGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // build matrices
    QMatrix4x4 model;
    model.rotate(m_timer.elapsed() / 1000.0f * 57.3f, { 0.5f, 1.0f, 0.0f });
    QMatrix4x4 view;
    view.lookAt({ 0,0,3 }, { 0,0,0 }, { 0,1,0 });
    QMatrix4x4 proj;
    proj.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    // bind and upload
    m_program.bind();
    m_program.setUniformValue("model", model);
    m_program.setUniformValue("view", view);
    m_program.setUniformValue("projection", proj);

    // draw
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    m_vao.release();

    // schedule next frame
    update();
}
