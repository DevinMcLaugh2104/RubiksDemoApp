#include "CubeCore.h"

CubeCore::CubeCore() {
    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z) {
                auto& c = cube[x][y][z];
                c.transform.setToIdentity();
                c.transform.translate(x - 1.0f, y - 1.0f, z - 1.0f);
                c.faceColors = {
                    QVector3D(1, 1, 1),
                    QVector3D(1, 1, 0),
                    QVector3D(0, 0, 1),
                    QVector3D(0, 1, 0),
                    QVector3D(1, 0, 0),
                    QVector3D(1, 0.5, 0)
                };
            }
}

Cubie& CubeCore::accessCubie(int x, int y, int z) {
    return cube[x][y][z];
}

const Cubie& CubeCore::getCubie(int x, int y, int z) const {
    return cube[x][y][z];
}

void CubeCore::rotateLayer(char axis, int index, bool clockwise) {
    // Simplified rotation: rotates transforms only
    QMatrix4x4 rotation;
    float angle = clockwise ? 90.0f : -90.0f;
    if (axis == 'x') rotation.rotate(angle, 1, 0, 0);
    if (axis == 'y') rotation.rotate(angle, 0, 1, 0);
    if (axis == 'z') rotation.rotate(angle, 0, 0, 1);

    for (int x = 0; x < SIZE; ++x)
        for (int y = 0; y < SIZE; ++y)
            for (int z = 0; z < SIZE; ++z) {
                if ((axis == 'x' && x == index) ||
                    (axis == 'y' && y == index) ||
                    (axis == 'z' && z == index)) {
                    cube[x][y][z].transform = rotation * cube[x][y][z].transform;
                }
            }
}

void CubeCore::moveRightLayer() {

}
void CubeCore::moveRightLayerPrime() {

}
void CubeCore::moveLeftLayer() {

}
void CubeCore::moveLeftLayerPrime() {

}
void CubeCore::moveUpLayer() {

}
void CubeCore::moveUpLayerPrime() {

}
void CubeCore::moveDownLayer() {

}
void CubeCore::moveDownLayerPrime() {

}
void CubeCore::moveFrontLayer() {

}
void CubeCore::moveFrontLayerPrime() {

}
void CubeCore::moveBackLayer() {

}
void CubeCore::moveBackLayerPrime() {

}
void CubeCore::moveELayer() {

}
void CubeCore::moveELayerPrime() {

}
void CubeCore::moveMLayer() {

}
void CubeCore::moveMLayerPrime() {

}
void CubeCore::moveSLayer() {

}
void CubeCore::moveSLayerPrime() {

}
void CubeCore::moveRw() {

}
void CubeCore::moveRwPrime() {

}
void CubeCore::moveLw() {

}
void CubeCore::moveLwPrime() {

}
void CubeCore::moveX() {

}
void CubeCore::moveXPrime() {

}
void CubeCore::moveY() {

}
void CubeCore::moveYPrime() {

}
void CubeCore::moveZ() {

}
void CubeCore::moveZPrime() {

}