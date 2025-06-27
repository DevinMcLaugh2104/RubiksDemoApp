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
#include <qgridlayout.h>
#include <iostream>

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
    void openPenaltyDialog(int index);
    void openSettingsDialog(); 
    void rewriteTable();

private:
    // scramble logic
    QString generateScramble(int length);
    QVector<QString> m_scramblesVec;
    int m_currentScrambleIndex = -1;

    // main stopwatch
    QElapsedTimer m_elapsed;
    QTimer* m_update;
    bool m_running;

    // hold-to-start logic
    QElapsedTimer m_holdTimer;
    bool m_holdActive;
    int m_timerValue = 1000;

    // UI
    QLabel* m_scrambleLabel;
    QLabel* m_timerLabel;
    QLabel* m_instructionLabel;
    QTableWidget* m_table;
    int rowIdx = -1;

    // statistics
    QVBoxLayout* m_statisticLayout;
    QLabel* m_bestSolve;
    QLabel* m_currentAo5;
    QLabel* m_bestAo5;
    double m_currentSolveTime = 0.000;
    double m_bestSolveTime = 0.000;
    double m_currentAo5Time = 0.000;
    double m_bestAo5Time = 0.000;
    QVector<double> solvesVec;
    QVector<double> solvesVecRawData;

    // penalty logic
    int penaltyIdx = -1;

    QPushButton* cubeButton;
    QPushButton* settingsButton;
    QPushButton* prevScrambleButton;
    QPushButton* nextScrambleButton;

    CubeGLWidget* m_glWidget;
    QColor m_backgroundColor = QColor(Qt::white);

    void prevScramble();
    void nextScramble();

    void scrambleCube(CubeGLWidget& cube);

    void calcBestSolve();
    void calcCurrentAo5();
    void calcBestAo5();
};

#endif 
