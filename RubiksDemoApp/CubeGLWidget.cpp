#include "CubeGLWidget.h"
#include <QOpenGLShader>
#include <QMouseEvent>
#include <iostream>

// ---------- Vertex Shader ----------
static const char* vShader = R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vWorldNormal;       // normal AFTER cubie rotation
flat out vec3 vIdx;          // logical cubie index (debug / pick)

uniform mat4 model, view, projection;
uniform vec3 cubieIndex;

void main()
{
    // model is a pure rigid-body transform so mat3(model) is fine
    vWorldNormal = mat3(model) * aNormal;
    vIdx         = cubieIndex;
    gl_Position  = projection * view * model * vec4(aPos, 1.0);
})";

// ---------- Fragment Shader ----------
static const char* fShader = R"(#version 330 core
in  vec3 vWorldNormal;
flat in vec3 vIdx;
out vec4 FragColor;

uniform vec3 faceColors[6];            // UP, DOWN, LEFT, RIGHT, FRONT, BACK

void main()
{
    vec3 n   = normalize(vWorldNormal);
    vec3 col = vec3(0.05);             // default “dark”

    // pick colour based on the *current* outward direction
    if      (n.y >  0.9) col = faceColors[0]; // UP    (+Y)
    else if (n.y < -0.9) col = faceColors[1]; // DOWN  (–Y)
    else if (n.x < -0.9) col = faceColors[2]; // LEFT  (–X)
    else if (n.x >  0.9) col = faceColors[3]; // RIGHT (+X)
    else if (n.z >  0.9) col = faceColors[4]; // FRONT (+Z)
    else if (n.z < -0.9) col = faceColors[5]; // BACK  (–Z)

    FragColor = vec4(col, 1.0);
})";

// ---------- Cube Geometry ----------
static const float verts[] = {
    // pos              // normal
    -0.5f,-0.5f,-0.5f,  0,0,-1,   0.5f,-0.5f,-0.5f,  0,0,-1,
     0.5f, 0.5f,-0.5f,  0,0,-1,  -0.5f, 0.5f,-0.5f,  0,0,-1,

    -0.5f,-0.5f, 0.5f,  0,0, 1,   0.5f,-0.5f, 0.5f,  0,0, 1,
     0.5f, 0.5f, 0.5f,  0,0, 1,  -0.5f, 0.5f, 0.5f,  0,0, 1,

    -0.5f,-0.5f,-0.5f, -1,0, 0,  -0.5f, 0.5f,-0.5f, -1,0, 0,
    -0.5f, 0.5f, 0.5f, -1,0, 0,  -0.5f,-0.5f, 0.5f, -1,0, 0,

     0.5f,-0.5f,-0.5f,  1,0, 0,   0.5f, 0.5f,-0.5f,  1,0, 0,
     0.5f, 0.5f, 0.5f,  1,0, 0,   0.5f,-0.5f, 0.5f,  1,0, 0,

    -0.5f,-0.5f,-0.5f,  0,-1,0,  -0.5f,-0.5f, 0.5f,  0,-1,0,
     0.5f,-0.5f, 0.5f,  0,-1,0,   0.5f,-0.5f,-0.5f,  0,-1,0,

    -0.5f, 0.5f,-0.5f,  0, 1,0,  -0.5f, 0.5f, 0.5f,  0, 1,0,
     0.5f, 0.5f, 0.5f,  0, 1,0,   0.5f, 0.5f,-0.5f,  0, 1,0
};

static const unsigned idx[] = {
     0, 1, 2, 2, 3, 0,   4, 5, 6, 6, 7, 4,
     8, 9,10,10,11, 8,  12,13,14,14,15,12,
    16,17,18,18,19,16,  20,21,22,22,23,20
};

// ---------- Constructor / Destructor ----------
CubeGLWidget::CubeGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

CubeGLWidget::~CubeGLWidget()
{
    makeCurrent();
    m_ebo.destroy();  m_vbo.destroy();  m_vao.destroy();
    m_prog.removeAllShaders();
    doneCurrent();
}

// ---------- GL initialisation ----------
void CubeGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    m_prog.addShaderFromSourceCode(QOpenGLShader::Vertex, vShader);
    m_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, fShader);
    m_prog.link();

    m_vao.create();  m_vao.bind();

    m_vbo.create();  m_vbo.bind();
    m_vbo.allocate(verts, sizeof(verts));

    m_ebo.create();  m_ebo.bind();
    m_ebo.allocate(idx, sizeof(idx));

    const int stride = 6 * sizeof(float);
    m_prog.enableAttributeArray(0);
    m_prog.setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);
    m_prog.enableAttributeArray(1);
    m_prog.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, stride);

    m_vao.release();
    m_prog.release();
}

void CubeGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_proj.setToIdentity();
    m_proj.perspective(45.f, float(w) / h, 0.1f, 100.f);
}

// ---------- Drawing ----------
void CubeGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_view.setToIdentity();
    m_view.translate(0, 0, -10);
    m_view.rotate(m_xRot, 1, 0, 0);
    m_view.rotate(m_yRot, 0, 1, 0);

    m_pickProj = m_proj;
    m_pickView = m_view;

    QMatrix4x4 cubeRot;
    cubeRot.rotate(m_xRot, 1, 0, 0);
    cubeRot.rotate(m_yRot, 0, 1, 0);
    m_cube.setOrientation(cubeRot);

    m_prog.bind();
    m_prog.setUniformValue("view", m_view);
    m_prog.setUniformValue("projection", m_proj);

    m_vao.bind();
    for (int x = 0; x < RubiksCube::SIZE; ++x)
        for (int y = 0; y < RubiksCube::SIZE; ++y)
            for (int z = 0; z < RubiksCube::SIZE; ++z)
            {
                const auto& c = m_cube.getCubie(x, y, z);
                m_prog.setUniformValue("model", c.transform);
                m_prog.setUniformValue("cubieIndex", c.index);
                m_prog.setUniformValueArray("faceColors",
                    c.faceColors.data(), 6);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            }
    m_vao.release();
    m_prog.release();
}

void CubeGLWidget::mousePressEvent(QMouseEvent* e)
{
    float x = (2.f * e->position().x()) / width() - 1.f;
    float y = 1.f - (2.f * e->position().y()) / height();
    QVector4D clip(x, y, -1, 1);
    QVector4D eye = m_pickProj.inverted() * clip; eye.setZ(-1); eye.setW(0);
    QVector3D ray = (m_pickView.inverted() * eye).toVector3D().normalized();

    m_rotating = !intersectsCube(m_camPos, ray);
    m_lastPos = e->pos();
}

void CubeGLWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (m_rotating && (e->buttons() & Qt::LeftButton))
    {
        rotateCube(e->position().x() - m_lastPos.x(),
            e->position().y() - m_lastPos.y());
        m_lastPos = e->pos();
        update();
    }
}

void CubeGLWidget::rotateCube(int dx, int dy) { m_xRot += dy; m_yRot += dx; }

bool CubeGLWidget::intersectsCube(const QVector3D& O, const QVector3D& D)
{
    QVector3D mn(-1, -1, -1), mx(1, 1, 1);
    float tmin = -1e20f, tmax = 1e20f;
    for (int i = 0; i < 3; ++i)
    {
        float inv = 1.f / D[i];
        float t0 = (mn[i] - O[i]) * inv, t1 = (mx[i] - O[i]) * inv;
        if (inv < 0) std::swap(t0, t1);
        tmin = std::max(tmin, t0);
        tmax = std::min(tmax, t1);
        if (tmax < tmin) return false;
    }
    return true;
}

void CubeGLWidget::syncCubeOrientation()
{
    QMatrix4x4 model;
    m_xRot = 30.0f;
    m_yRot = -45.0f;
    m_cube.setOrientation(model);
}

void CubeGLWidget::resetCube() {
    m_cube = RubiksCube();          
    m_xRot = 30.0f;            
    m_yRot = -45.0f;
    update();
}

void CubeGLWidget::moveUpLayer() { syncCubeOrientation(); m_cube.U();  update(); }
void CubeGLWidget::moveUpLayerPrime() { syncCubeOrientation(); m_cube.Up(); update(); }
void CubeGLWidget::moveUpLayer2() { syncCubeOrientation(); m_cube.U2(); update(); }
void CubeGLWidget::moveDownLayer() { syncCubeOrientation(); m_cube.D();  update(); }
void CubeGLWidget::moveDownLayerPrime() { syncCubeOrientation(); m_cube.Dp(); update(); }
void CubeGLWidget::moveDownLayer2() { syncCubeOrientation(); m_cube.D2(); update(); }
void CubeGLWidget::moveRightLayer() { syncCubeOrientation(); m_cube.R();  update(); }
void CubeGLWidget::moveRightLayerPrime() { syncCubeOrientation(); m_cube.Rp(); update(); }
void CubeGLWidget::moveRightLayer2() { syncCubeOrientation(); m_cube.R2(); update(); }
void CubeGLWidget::moveLeftLayer() { syncCubeOrientation(); m_cube.L(); update(); }
void CubeGLWidget::moveLeftLayerPrime() { syncCubeOrientation(); m_cube.Lp(); update(); }
void CubeGLWidget::moveLeftLayer2() { syncCubeOrientation(); m_cube.L2(); update(); }
void CubeGLWidget::moveFrontLayer() { syncCubeOrientation(); m_cube.F();  update(); }
void CubeGLWidget::moveFrontLayerPrime() { syncCubeOrientation(); m_cube.Fp(); update(); }
void CubeGLWidget::moveFrontLayer2() { syncCubeOrientation(); m_cube.F2(); update(); }
void CubeGLWidget::moveBackLayer() { syncCubeOrientation(); m_cube.B();  update(); }
void CubeGLWidget::moveBackLayerPrime() { syncCubeOrientation(); m_cube.Bp(); update(); }
void CubeGLWidget::moveBackLayer2() { syncCubeOrientation(); m_cube.B2(); update(); }

