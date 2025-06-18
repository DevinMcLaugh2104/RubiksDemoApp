#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <array>

class RubiksCube {
public:
    static constexpr int SIZE = 3;
    static constexpr float GAP = 1.02f;

    enum Face { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };

    struct Cubie {
        QMatrix4x4 transform;
        std::array<QVector3D, 6> faceColors; // indexed by Face
        QVector3D index; // logical (x,y,z) position
    };

    RubiksCube();

    const Cubie& getCubie(int x, int y, int z) const { return cube[x][y][z]; }
    Cubie& accessCubie(int x, int y, int z) { return cube[x][y][z]; }

    void rotateLayer(char axis, int index, bool clockwise);

    // Moves
    void R() { rotateLayer('X', 2, true); }
    void Rp() { rotateLayer('X', 2, false); }
    void L() { rotateLayer('X', 0, false); }
    void Lp() { rotateLayer('X', 0, true); }
    void U() { rotateLayer('Y', 2, true); }
    void Up() { rotateLayer('Y', 2, false); }
    void D() { rotateLayer('Y', 0, false); }
    void Dp() { rotateLayer('Y', 0, true); }
    void F() { rotateLayer('Z', 2, true); }
    void Fp() { rotateLayer('Z', 2, false); }
    void B() { rotateLayer('Z', 0, false); }
    void Bp() { rotateLayer('Z', 0, true); }

private:
    Cubie cube[SIZE][SIZE][SIZE];

    static void rotateFacesX(Cubie& c, bool cw);
    static void rotateFacesY(Cubie& c, bool cw);
    static void rotateFacesZ(Cubie& c, bool cw);
};


