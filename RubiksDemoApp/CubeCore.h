#pragma once
#include <array>
#include <QMatrix4x4>
#include <QVector3D>

struct Cubie {
    QMatrix4x4 transform;
    std::array<QVector3D, 6> faceColors; // Up, Down, Left, Right, Front, Back
};

class CubeCore {
public:
    static constexpr int SIZE = 3;
    Cubie cube[SIZE][SIZE][SIZE];

    CubeCore();
    void rotateLayer(char axis, int index, bool clockwise);
    const Cubie& getCubie(int x, int y, int z) const;
    Cubie& accessCubie(int x, int y, int z);

    // Original movement functions (keep them for solving logic)
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