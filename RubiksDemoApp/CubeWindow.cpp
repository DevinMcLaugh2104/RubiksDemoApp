#include "CubeWindow.h"
#include "CubeGLWidget.h"
#include <QVBoxLayout>

CubeWindow::CubeWindow(QWidget* parent)
    : QMainWindow(parent), glWidget(new CubeGLWidget(this))
{
    auto* container = new QWidget(this);
    auto* layout = new QVBoxLayout(container);
    layout->addWidget(glWidget);
    setCentralWidget(container);
    resize(800, 600);
    setWindowTitle("3D Cube");
}
