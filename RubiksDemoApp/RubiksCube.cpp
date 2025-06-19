#include "RubiksCube.h"
#include <algorithm>

// Sticker colors
static const QVector3D WHITE{ 1, 1, 1 };
static const QVector3D YELLOW{ 1, 1, 0 };
static const QVector3D ORANGE{ 1, 0.5, 0 };
static const QVector3D RED{ 1, 0, 0 };
static const QVector3D GREEN{ 0, 1, 0 };
static const QVector3D BLUE{ 0, 0, 1 };
static const QVector3D DARK{ 0.05, 0.05, 0.05 };

RubiksCube::RubiksCube() {
    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z) {
                Cubie& c = cube[x][y][z];
                c.index = QVector3D(x, y, z);
                c.transform.setToIdentity();
                c.transform.translate((x - 1) * GAP, (y - 1) * GAP, (z - 1) * GAP);
                c.faceColors.fill(DARK);

                if (y == 2) c.faceColors[UP] = WHITE;
                if (y == 0) c.faceColors[DOWN] = YELLOW;
                if (x == 0) c.faceColors[LEFT] = ORANGE;
                if (x == 2) c.faceColors[RIGHT] = RED;
                if (z == 2) c.faceColors[FRONT] = GREEN;
                if (z == 0) c.faceColors[BACK] = BLUE;
            }
}

RubiksCube::AxisInfo RubiksCube::mapWorldDirToAxis(const QVector3D& worldDir) const {
    QVector3D local = m_orientation.inverted().mapVector(worldDir);
    float ax = std::fabs(local.x()), ay = std::fabs(local.y()), az = std::fabs(local.z());

    AxisInfo out{};
    if (ax >= ay && ax >= az) {
        out.axis = 'X';
        out.positive = local.x() > 0;
    }
    else if (ay >= ax && ay >= az) {
        out.axis = 'Y';
        out.positive = local.y() > 0;
    }
    else {
        out.axis = 'Z';
        out.positive = local.z() > 0;
    }
    return out;
}

void RubiksCube::rotateLayer(char axis, int index, bool clockwise) {
    std::array<std::array<std::array<Cubie, SIZE>, SIZE>, SIZE> original = cube;

    auto rotate90 = [&](QMatrix4x4& mat, const QVector3D& axisVec) {
        QMatrix4x4 rot;
        rot.rotate(clockwise ? 90 : -90, axisVec);
        mat = rot * mat;
        };

    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z) {
                if ((axis == 'X' && x == index) ||
                    (axis == 'Y' && y == index) ||
                    (axis == 'Z' && z == index)) {

                    int nx = x, ny = y, nz = z;
                    if (axis == 'X') {
                        ny = clockwise ? SIZE - 1 - z : z;
                        nz = clockwise ? y : SIZE - 1 - y;
                    }
                    else if (axis == 'Y') {
                        nx = clockwise ? z : SIZE - 1 - z;
                        nz = clockwise ? SIZE - 1 - x : x;
                    }
                    else if (axis == 'Z') {
                        nx = clockwise ? SIZE - 1 - y : y;
                        ny = clockwise ? x : SIZE - 1 - x;
                    }

                    cube[nx][ny][nz] = original[x][y][z];
                    rotate90(cube[nx][ny][nz].transform,
                        axis == 'X' ? QVector3D(1, 0, 0) :
                        axis == 'Y' ? QVector3D(0, 1, 0) :
                        QVector3D(0, 0, 1));

                    if (axis == 'X') rotateFacesX(cube[nx][ny][nz], clockwise);
                    if (axis == 'Y') rotateFacesY(cube[nx][ny][nz], clockwise);
                    if (axis == 'Z') rotateFacesZ(cube[nx][ny][nz], clockwise);
                }
            }
}

void RubiksCube::rotateFacesX(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        QVector3D temp = f[UP];
        f[UP] = f[BACK];
        f[BACK] = f[DOWN];
        f[DOWN] = f[FRONT];
        f[FRONT] = temp;
    }
    else {
        QVector3D temp = f[UP];
        f[UP] = f[FRONT];
        f[FRONT] = f[DOWN];
        f[DOWN] = f[BACK];
        f[BACK] = temp;
    }
}

void RubiksCube::rotateFacesY(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        QVector3D temp = f[FRONT];
        f[FRONT] = f[LEFT];
        f[LEFT] = f[BACK];
        f[BACK] = f[RIGHT];
        f[RIGHT] = temp;
    }
    else {
        QVector3D temp = f[FRONT];
        f[FRONT] = f[RIGHT];
        f[RIGHT] = f[BACK];
        f[BACK] = f[LEFT];
        f[LEFT] = temp;
    }
}

void RubiksCube::rotateFacesZ(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        QVector3D temp = f[UP];
        f[UP] = f[RIGHT];
        f[RIGHT] = f[DOWN];
        f[DOWN] = f[LEFT];
        f[LEFT] = temp;
    }
    else {
        QVector3D temp = f[UP];
        f[UP] = f[LEFT];
        f[LEFT] = f[DOWN];
        f[DOWN] = f[RIGHT];
        f[RIGHT] = temp;
    }
}

void RubiksCube::U() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    int layer = up.positive ? 2 : 0;
    bool clockwise = !up.positive;
    rotateLayer(up.axis, layer, clockwise);
}

void RubiksCube::Up() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    int layer = up.positive ? 2 : 0;
    bool clockwise = up.positive;
    rotateLayer(up.axis, layer, clockwise);
}

void RubiksCube::D() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    int layer = up.positive ? 0 : 2;
    bool clockwise = up.positive;
    rotateLayer(up.axis, layer, clockwise);
}

void RubiksCube::Dp() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    int layer = up.positive ? 0 : 2;
    bool clockwise = !up.positive;
    rotateLayer(up.axis, layer, clockwise);
}

void RubiksCube::R() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo right = mapWorldDirToAxis(QVector3D(1, 0, 0));
    int layer = right.positive ? 2 : 0;
    bool clockwise = (up.positive ^ right.positive);
    rotateLayer(right.axis, layer, clockwise);
}

void RubiksCube::Rp() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo right = mapWorldDirToAxis(QVector3D(1, 0, 0));
    int layer = right.positive ? 2 : 0;
    bool clockwise = !(up.positive ^ right.positive);
    rotateLayer(right.axis, layer, clockwise);
}

void RubiksCube::L() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo right = mapWorldDirToAxis(QVector3D(1, 0, 0));
    int layer = right.positive ? 0 : 2;
    bool clockwise = !(up.positive ^ right.positive);
    rotateLayer(right.axis, layer, clockwise);
}

void RubiksCube::Lp() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo right = mapWorldDirToAxis(QVector3D(1, 0, 0));
    int layer = right.positive ? 0 : 2;
    bool clockwise = (up.positive ^ right.positive);
    rotateLayer(right.axis, layer, clockwise);
}

void RubiksCube::F() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo front = mapWorldDirToAxis(QVector3D(0, 0, 1)); 
    int layer = front.positive ? 2 : 0;
    bool clockwise = (up.positive ^ front.positive);
    rotateLayer(front.axis, layer, clockwise);
}

void RubiksCube::Fp() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo front = mapWorldDirToAxis(QVector3D(0, 0, 1)); 
    int layer = front.positive ? 2 : 0;
    bool clockwise = !(up.positive ^ front.positive);
    rotateLayer(front.axis, layer, clockwise);
}

void RubiksCube::B() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo front = mapWorldDirToAxis(QVector3D(0, 0, 1)); 
    int layer = front.positive ? 0 : 2;
    bool clockwise = !(up.positive ^ front.positive);
    rotateLayer(front.axis, layer, clockwise);
}

void RubiksCube::Bp() {
    AxisInfo up = mapWorldDirToAxis(QVector3D(0, 1, 0));
    AxisInfo front = mapWorldDirToAxis(QVector3D(0, 0, 1)); 
    int layer = front.positive ? 0 : 2;
    bool clockwise = (up.positive ^ front.positive);
    rotateLayer(front.axis, layer, clockwise);
}

