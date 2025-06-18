#pragma once

#include <array>
#include <QMatrix4x4>
#include <QVector3D>

constexpr float GAP = 1.02f; // spacing between cubies for visual clarity

// Enum for face indices
enum Face {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

struct Cubie {
    QMatrix4x4 transform;
    std::array<QVector3D, 6> faceColors; // Up, Down, Left, Right, Front, Back
    QVector3D index; // <-- ADD THIS
};


class CubeCore {
public:
    static constexpr int SIZE = 3;
    Cubie cube[SIZE][SIZE][SIZE];

    CubeCore();

    // Accessors
    const Cubie& getCubie(int x, int y, int z) const;
    Cubie& accessCubie(int x, int y, int z);

    // General layer rotation function
    void rotateLayer(char axis, int index, bool clockwise);

    // WCA face turns
    void moveRightLayer();
    void moveRightLayerPrime();
    void moveLeftLayer();
    void moveLeftLayerPrime();
    void moveUpLayer();
    void moveUpLayerPrime();
    void moveDownLayer();
    void moveDownLayerPrime();
    void moveFrontLayer();
    void moveFrontLayerPrime();
    void moveBackLayer();
    void moveBackLayerPrime();

    // Extended moves (to be implemented as needed)
    void moveELayer();
    void moveELayerPrime();
    void moveMLayer();
    void moveMLayerPrime();
    void moveSLayer();
    void moveSLayerPrime();
    void moveRw();
    void moveRwPrime();
    void moveLw();
    void moveLwPrime();
    void moveX();
    void moveXPrime();
    void moveY();
    void moveYPrime();
    void moveZ();
    void moveZPrime();
};
