#include "CubeGLWidget.h"
#include <QOpenGLShader>

// ---------------- GLSL ----------------
static const char* vShader = R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;            // world-space normal
flat out vec3 vIdx;          // cubie logical index (0,1,2)

uniform mat4 model, view, projection;
uniform vec3 cubieIndex;     // set from C++

void main()
{
    vec4 worldPos = model * vec4(aPos,1.0);
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vIdx    = cubieIndex;
    gl_Position = projection * view * worldPos;
})";

static const char* fShader = R"(#version 330 core
in vec3 vNormal;
flat in vec3 vIdx;
out vec4 FragColor;

const vec3 white  = vec3(1,1,1);    // +Y
const vec3 yellow = vec3(1,1,0);    // –Y
const vec3 green  = vec3(0,1,0);    // +Z
const vec3 blue   = vec3(0,0,1);    // –Z
const vec3 red    = vec3(1,0,0);    // +X
const vec3 orange = vec3(1,0.5,0);  // –X
const vec3 dark   = vec3(0.05);     // internal faces

void main()
{
    vec3 n = normalize(vNormal);
    vec3 col = dark;                    // default

    // show stickers only on the outer layer + outward-facing side
    if (n.y >  0.9 && vIdx.y > 1.5) col = white;
    else if (n.y < -0.9 && vIdx.y < 0.5) col = yellow;
    else if (n.z >  0.9 && vIdx.z > 1.5) col = green;
    else if (n.z < -0.9 && vIdx.z < 0.5) col = blue;
    else if (n.x >  0.9 && vIdx.x > 1.5) col = red;
    else if (n.x < -0.9 && vIdx.x < 0.5) col = orange;

    FragColor = vec4(col,1.0);
})";

// ------------ cube geometry (24 verts, 36 indices) ----------
static const float verts[] = { // pos              // normal
    // –Z
    -0.5f,-0.5f,-0.5f,  0,0,-1,
     0.5f,-0.5f,-0.5f,  0,0,-1,
     0.5f, 0.5f,-0.5f,  0,0,-1,
    -0.5f, 0.5f,-0.5f,  0,0,-1,
    // +Z
    -0.5f,-0.5f, 0.5f,  0,0,1,
     0.5f,-0.5f, 0.5f,  0,0,1,
     0.5f, 0.5f, 0.5f,  0,0,1,
    -0.5f, 0.5f, 0.5f,  0,0,1,
    // –X
    -0.5f,-0.5f,-0.5f, -1,0,0,
    -0.5f, 0.5f,-0.5f, -1,0,0,
    -0.5f, 0.5f, 0.5f, -1,0,0,
    -0.5f,-0.5f, 0.5f, -1,0,0,
    // +X
     0.5f,-0.5f,-0.5f,  1,0,0,
     0.5f, 0.5f,-0.5f,  1,0,0,
     0.5f, 0.5f, 0.5f,  1,0,0,
     0.5f,-0.5f, 0.5f,  1,0,0,
     // –Y
     -0.5f,-0.5f,-0.5f,  0,-1,0,
     -0.5f,-0.5f, 0.5f,  0,-1,0,
      0.5f,-0.5f, 0.5f,  0,-1,0,
      0.5f,-0.5f,-0.5f,  0,-1,0,
      // +Y
      -0.5f, 0.5f,-0.5f,  0,1,0,
      -0.5f, 0.5f, 0.5f,  0,1,0,
       0.5f, 0.5f, 0.5f,  0,1,0,
       0.5f, 0.5f,-0.5f,  0,1,0
};

static const unsigned idx[] = {
     0, 1, 2, 2, 3, 0,   // –Z
     4, 5, 6, 6, 7, 4,   // +Z
     8, 9,10,10,11, 8,   // –X
    12,13,14,14,15,12,   // +X
    16,17,18,18,19,16,   // –Y
    20,21,22,22,23,20    // +Y
};

// ------------ ctor / dtor ------------
CubeGLWidget::CubeGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}
CubeGLWidget::~CubeGLWidget()
{
    makeCurrent();
    m_ebo.destroy(); m_vbo.destroy(); m_vao.destroy();
    m_prog.removeAllShaders();
    doneCurrent();
}

// ------------ GL init ---------------
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

// ------------ draw ------------------
void CubeGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera
    m_view.setToIdentity();
    m_view.translate(0, 0, -10);
    m_view.rotate(m_xRot, 1, 0, 0);
    m_view.rotate(m_yRot, 0, 1, 0);

    // store for picking
    m_pickProj = m_proj;
    m_pickView = m_view;

    m_prog.bind();
    m_prog.setUniformValue("view", m_view);
    m_prog.setUniformValue("projection", m_proj);

    m_vao.bind();
    for (int x = 0; x < RubiksCube::SIZE; ++x)
        for (int y = 0; y < RubiksCube::SIZE; ++y)
            for (int z = 0; z < RubiksCube::SIZE; ++z)
            {
                const RubiksCube::Cubie& c = m_cube.getCubie(x, y, z);
                m_prog.setUniformValue("model", c.transform);
                m_prog.setUniformValue("cubieIndex", c.index);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            }
    m_vao.release();
    m_prog.release();
}

// ------------ mouse -----------------
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

// ------------ helpers ---------------
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
