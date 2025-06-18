#include "RubiksCube.h"
#include <algorithm>

// Sticker colors
static const QVector3D WHITE{ 1, 1,   1 };
static const QVector3D YELLOW{ 1, 1,   0 };
static const QVector3D ORANGE{ 1, 0.5, 0 };
static const QVector3D RED{ 1, 0,   0 };
static const QVector3D GREEN{ 0, 1,   0 };
static const QVector3D BLUE{ 0, 0,   1 };
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

void RubiksCube::rotateLayer(char axis, int index, bool cw) {
    Cubie tmp[SIZE][SIZE];

    auto map = [cw](int a, int b) {
        return cw ? std::make_pair(SIZE - 1 - b, a)
            : std::make_pair(b, SIZE - 1 - a);
        };

    if (axis == 'X') {
        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z)
                tmp[y][z] = cube[index][y][z];

        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z) {
                auto [ny, nz] = map(y, z);
                cube[index][ny][nz] = tmp[y][z];
                cube[index][ny][nz].transform.rotate(cw ? 90.f : -90.f, 1, 0, 0);
                rotateFacesX(cube[index][ny][nz], cw);
            }
    }
    else if (axis == 'Y') {
        for (int x = 0; x < SIZE; ++x)
            for (int z = 0; z < SIZE; ++z)
                tmp[x][z] = cube[x][index][z];

        for (int x = 0; x < SIZE; ++x)
            for (int z = 0; z < SIZE; ++z) {
                auto [nx, nz] = map(x, z);
                cube[nx][index][nz] = tmp[x][z];
                cube[nx][index][nz].transform.rotate(cw ? 90.f : -90.f, 0, 1, 0);
                rotateFacesY(cube[nx][index][nz], cw);
            }
    }
    else if (axis == 'Z') {
        for (int x = 0; x < SIZE; ++x)
            for (int y = 0; y < SIZE; ++y)
                tmp[x][y] = cube[x][y][index];

        for (int x = 0; x < SIZE; ++x)
            for (int y = 0; y < SIZE; ++y) {
                auto [nx, ny] = map(x, y);
                cube[nx][ny][index] = tmp[x][y];
                cube[nx][ny][index].transform.rotate(cw ? 90.f : -90.f, 0, 0, 1);
                rotateFacesZ(cube[nx][ny][index], cw);
            }
    }
}

void RubiksCube::rotateFacesX(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        std::swap(f[UP], f[FRONT]);
        std::swap(f[FRONT], f[DOWN]);
        std::swap(f[DOWN], f[BACK]);
    }
    else {
        std::swap(f[UP], f[BACK]);
        std::swap(f[BACK], f[DOWN]);
        std::swap(f[DOWN], f[FRONT]);
    }
}

void RubiksCube::rotateFacesY(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        std::swap(f[FRONT], f[RIGHT]);
        std::swap(f[RIGHT], f[BACK]);
        std::swap(f[BACK], f[LEFT]);
    }
    else {
        std::swap(f[FRONT], f[LEFT]);
        std::swap(f[LEFT], f[BACK]);
        std::swap(f[BACK], f[RIGHT]);
    }
}

void RubiksCube::rotateFacesZ(Cubie& c, bool cw) {
    auto& f = c.faceColors;
    if (cw) {
        std::swap(f[UP], f[LEFT]);
        std::swap(f[LEFT], f[DOWN]);
        std::swap(f[DOWN], f[RIGHT]);
    }
    else {
        std::swap(f[UP], f[RIGHT]);
        std::swap(f[RIGHT], f[DOWN]);
        std::swap(f[DOWN], f[LEFT]);
    }
}

