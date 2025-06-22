#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <array>
#include <cmath>

class RubiksCube {
public:
    static constexpr int SIZE = 3;
    static constexpr float GAP = 1.02f;          // distance between cubie centres

    enum Face { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };

    struct Cubie {
        QMatrix4x4                transform;      // local model matrix
        std::array<QVector3D, 6>  faceColors;     // indexed by Face
        QVector3D                 index;          // logical (x,y,z) position
    };

    RubiksCube();

    // ?? orientation ???????????????????????????????????????????????????????????
    /// Call every time the whole cube changes orientation (mouse drag, etc.).
    void setOrientation(const QMatrix4x4& cubeToWorld) { m_orientation = cubeToWorld; }

    // ?? high-level moves, always relative to the *current* top face ???????????
    void R();  void Rp();  void R2();
    void L();  void Lp();  void L2();
    void U();  void Up();  void U2();
    void D();  void Dp();  void D2();
    void F();  void Fp();  void F2();
    void B();  void Bp();  void B2();

    // ?? low-level helpers (fixed global axes) – still useful internally ???????
    void rotateLayer(char axis, int index, bool clockwise);

    // ?? read access to individual cubies (for drawing) ????????????????????????
    const Cubie& getCubie(int x, int y, int z) const { return cube[x][y][z]; }
    Cubie& accessCubie(int x, int y, int z) { return cube[x][y][z]; }

private:
    // Helper that maps a world-space direction to this cube’s local axis (+/-)
    struct AxisInfo { char axis; bool positive; };
    AxisInfo mapWorldDirToAxis(const QVector3D& worldDir) const;

    std::array<std::array<std::array<Cubie, SIZE>, SIZE>, SIZE> cube;
    QMatrix4x4 m_orientation{ QMatrix4x4() };    // cube ? world

    static void rotateFacesX(Cubie& c, bool cw);
    static void rotateFacesY(Cubie& c, bool cw);
    static void rotateFacesZ(Cubie& c, bool cw);
};


