#pragma once
#include <QMainWindow>
class CubeGLWidget;

class CubeWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit CubeWindow(QWidget* parent = nullptr);
private:
    CubeGLWidget* glWidget;
};
