#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CubeGLWidget.h"
#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
 

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onUpdateTimer();
    void onShowCube();
    void openSettingsDialog();

private:
    QString generateScramble(int length);

    // main stopwatch
    QElapsedTimer   m_elapsed;
    QTimer* m_update;
    bool            m_running;

    // hold-to-start logic
    QElapsedTimer   m_holdTimer;
    bool            m_holdActive;
    int             m_timerValue = 1000;

    // UI
    QLabel* m_scrambleLabel;
    QLabel* m_label;
    QTableWidget* m_table;

    QPushButton* cubeButton;
    QPushButton* settingsButton;

    CubeGLWidget* m_glWidget;
    QColor m_backgroundColor = QColor(Qt::white);

};

#endif // MAINWINDOW_H
