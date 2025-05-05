#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>

// How did i lose this header file from the list?

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

private:
    QString generateScramble(int length);

    // main stopwatch
    QElapsedTimer   m_elapsed;
    QTimer* m_update;
    bool            m_running;

    // hold-to-start logic
    QElapsedTimer   m_holdTimer;
    bool            m_holdActive;

    // UI
    QLabel* m_scrambleLabel;
    QLabel* m_label;
    QTableWidget* m_table;

    QPushButton* cubeButton;
};

#endif // MAINWINDOW_H
