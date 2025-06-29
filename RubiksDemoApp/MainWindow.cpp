#include "mainwindow.h"
#include "CubeWindow.h"
#include "CubeGLWidget.h"
#include "PenaltyDialog.h"
#include "SettingsDialog.h"
#include <QApplication>rrvba3
#include <QEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <QPushButton>
#include <QLabel>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_update(new QTimer(this)), m_running(false), m_holdActive(false)
{
    qApp->installEventFilter(this);

    m_scrambleLabel = new QLabel(this);
    m_scrambleLabel->setAlignment(Qt::AlignCenter);
    m_scrambleLabel->setWordWrap(true);
    QString firstScramble = generateScramble(20);
    m_scramblesVec.append(firstScramble);
    m_currentScrambleIndex = 0;
    m_scrambleLabel->setText(firstScramble);

    QFont scrambleFont = m_scrambleLabel->font();
    scrambleFont.setPointSize(30);
    m_scrambleLabel->setFont(scrambleFont);

    m_timerLabel = new QLabel("0.000", this);
    m_timerLabel->setAlignment(Qt::AlignCenter);
    QFont timerFont = m_timerLabel->font();
    timerFont.setPointSize(60);
    timerFont.setBold(true);
    m_timerLabel->setFont(timerFont);

    m_instructionLabel = new QLabel("Hold Space ≥1 s, then release to start", this);
    m_instructionLabel->setAlignment(Qt::AlignCenter);
    QFont instrFont = m_instructionLabel->font();
    instrFont.setPointSize(14);
    m_instructionLabel->setFont(instrFont);

    auto* timerBox = new QGroupBox(this);
    auto* timerLay = new QVBoxLayout(timerBox);
    timerLay->addWidget(m_timerLabel);
    timerLay->addWidget(m_instructionLabel);
    timerLay->addStretch();
    timerBox->setStyleSheet("border: none;");
    timerBox->setFixedSize(500, 300);

    m_solvesTable = new QTableWidget(this);
    m_solvesTable->setColumnCount(3);
    m_solvesTable->setHorizontalHeaderLabels({ "#", "Time (s)", "Penalty" });
    m_solvesTable->setFixedWidth(320);
    m_solvesTable->setFixedHeight(1200);
    m_solvesTable->horizontalHeader()->setStretchLastSection(true);
    m_solvesTable->verticalHeader()->setVisible(false);
    m_solvesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_solvesTable->setSelectionMode(QAbstractItemView::NoSelection);

    clearListButton = new QPushButton("Clear List", this);
    clearListButton->setFixedWidth(320);

    m_tableLayout = new QVBoxLayout;
    m_tableLayout->addWidget(m_solvesTable);
    m_tableLayout->addWidget(clearListButton);
    m_tableLayout->setContentsMargins(0, 0, 0, 0);
    m_tableLayout->setSpacing(0);

    auto* tableContainer = new QWidget(this);
    tableContainer->setLayout(m_tableLayout);
    tableContainer->setFixedWidth(320);
    tableContainer->setFixedHeight(1200);

    auto* tableWrapperLayout = new QVBoxLayout;
    tableWrapperLayout->addWidget(tableContainer, 0, Qt::AlignTop);
    tableWrapperLayout->addStretch();
    tableWrapperLayout->setContentsMargins(0, 0, 0, 0);

    auto* tableWrapper = new QWidget(this);
    tableWrapper->setLayout(tableWrapperLayout);

    cubeButton = new QPushButton("Open Cube", this);
    settingsButton = new QPushButton("Settings", this);

    m_statsLayout = new QVBoxLayout;
    QFont statisticsFont("Sans-Serif");
    statisticsFont.setPointSize(20);

    m_bestSolve = new QLabel("Best Solve: " + QString::number(m_bestSolveTime, 'f', 3));
    m_bestAo5 = new QLabel("Best Ao5: " + QString::number(m_bestAo5Time, 'f', 3));
    m_currentAo5 = new QLabel("Current Ao5: " + QString::number(m_currentAo5Time, 'f', 3));

    m_bestSolve->setFont(statisticsFont);
    m_bestAo5->setFont(statisticsFont);
    m_currentAo5->setFont(statisticsFont);

    m_statsLayout->addWidget(m_bestSolve);
    m_statsLayout->addWidget(m_bestAo5);
    m_statsLayout->addWidget(m_currentAo5);
    m_statsLayout->addWidget(cubeButton);
    m_statsLayout->addWidget(settingsButton);
 
    m_statsLayout->setContentsMargins(0, 0, 0, 0);
    m_statsLayout->setSpacing(0);

    auto* statsContainer = new QWidget(this);
    statsContainer->setLayout(m_statsLayout);
    statsContainer->setFixedWidth(320);
    statsContainer->setFixedHeight(320);
    statsContainer->setStyleSheet("background-color: rgba(255, 255, 255, 180);");

    auto* statsWrapperLayout = new QVBoxLayout;
    statsWrapperLayout->addWidget(statsContainer, 0, Qt::AlignTop);
    statsWrapperLayout->addStretch();
    statsWrapperLayout->setContentsMargins(0, 0, 0, 0);

    auto* statsWrapper = new QWidget(this);
    statsWrapper->setLayout(statsWrapperLayout);

    auto* central = new QWidget(this);
    auto* outerLayout = new QVBoxLayout(central);
    auto* contentLay = new QHBoxLayout;

    contentLay->addWidget(tableWrapper);
    contentLay->addStretch();
    contentLay->addWidget(timerBox);
    contentLay->addStretch();
    contentLay->addWidget(statsWrapper);

    outerLayout->addWidget(m_scrambleLabel);

    auto* scrambleNavLayout = new QHBoxLayout;
    scrambleNavLayout->setSpacing(8); 
    scrambleNavLayout->setContentsMargins(0, 0, 0, 0);
    scrambleNavLayout->setAlignment(Qt::AlignCenter);

    prevScrambleButton = new QPushButton("Prev Scramble", this);
    prevScrambleButton->setMaximumWidth(120);
    nextScrambleButton = new QPushButton("Next Scramble", this);
    nextScrambleButton->setMaximumWidth(120);

    scrambleNavLayout->addWidget(prevScrambleButton);
    scrambleNavLayout->addWidget(nextScrambleButton);

    auto* scrambleNavContainer = new QWidget(this);
    scrambleNavContainer->setLayout(scrambleNavLayout);

    outerLayout->addWidget(scrambleNavContainer);

    outerLayout->addLayout(contentLay);

    connect(cubeButton, &QPushButton::clicked, this, &MainWindow::onShowCube);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsDialog);
    connect(prevScrambleButton, &QPushButton::clicked, this, &MainWindow::prevScramble);
    connect(nextScrambleButton, &QPushButton::clicked, this, &MainWindow::nextScramble);
    connect(clearListButton, &QPushButton::clicked, this, &MainWindow::clearSolvesList);

    setCentralWidget(central);

    m_update->setInterval(50);
    connect(m_update, &QTimer::timeout, this, &MainWindow::onUpdateTimer);
}

MainWindow::~MainWindow() = default;

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if ((event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) &&
        static_cast<QKeyEvent*>(event)->isAutoRepeat())
        return false;

    if (event->type() == QEvent::KeyPress) {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Space && !m_running && !m_holdActive) {
            m_holdActive = true;
            m_holdTimer.restart();
            return true;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Space) {
            if (!m_running) {
                if (m_holdActive && m_holdTimer.elapsed() >= m_timerValue) {
                    m_elapsed.restart();
                    m_update->start();
                    m_running = true;
                    m_timerLabel->setText("0.000");
                    m_timerLabel->setStyleSheet("font-size: 80px; font-weight: bold;");
                    m_instructionLabel->hide();
                }
            }
            else {
                m_update->stop();
                m_running = false;
                double secs = m_elapsed.elapsed() / 1000.0;
                m_currentSolveTime = secs;
                solvesVec.push_back(secs); 
                solvesVecRawData.push_back(secs);

                rewriteTable(); 

                m_timerLabel->setText(QString::asprintf("%.3f s", secs));

                if (m_currentScrambleIndex < m_scramblesVec.size() - 1)
                    m_scramblesVec.resize(m_currentScrambleIndex + 1);

                QString newScramble = generateScramble(20);
                m_scramblesVec.push_back(newScramble);
                m_currentScrambleIndex = m_scramblesVec.size() - 1;
                m_scrambleLabel->setText(newScramble);

                calcBestSolve();
                calcCurrentAo5();
                calcBestAo5();
            }
            m_holdActive = false;
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

enum Axis { AxPosX, AxNegX, AxPosY, AxNegY, AxPosZ, AxNegZ };

static Axis detectTopAxis(float xDeg, float yDeg)
{
    QMatrix4x4 R;
    R.rotate(xDeg, 1, 0, 0);
    R.rotate(yDeg, 0, 1, 0);
    QMatrix4x4 inv = R.inverted();

    struct Test { QVector3D v; Axis a; };
    std::array<Test, 6> axes{ {
        {{ 1, 0, 0}, AxPosX}, {{-1, 0, 0}, AxNegX},
        {{ 0, 1, 0}, AxPosY}, {{ 0,-1, 0}, AxNegY},
        {{ 0, 0, 1}, AxPosZ}, {{ 0, 0,-1}, AxNegZ}
    } };

    Axis bestA = AxPosY;
    float best = -2;                 
    for (auto& t : axes) {
        float d = QVector3D::dotProduct(inv.map(t.v), QVector3D(0, 1, 0));
        if (d > best) {
            best = d;
            bestA = t.a;
        }
    }
    return bestA;
}

void MainWindow::onShowCube()
{
    auto* cw = new CubeWindow(nullptr);
    cw->setAttribute(Qt::WA_DeleteOnClose);

    auto* cWidget = new QWidget(cw);
    auto* rootLay = new QVBoxLayout(cWidget);
    auto* btnGrid = new QGridLayout;
    auto* cubeW = new CubeGLWidget(cWidget);

    auto doUPressed = [cubeW, this] {
        Axis top = detectTopAxis(cubeW->xRotation(), cubeW->yRotation());
        switch (top) {
        case AxPosY: cubeW->moveUpLayer();               break;         // white on top
        case AxNegY: cubeW->moveDownLayerPrime();        break;         // yellow on top
        case AxPosZ: cubeW->moveFrontLayer();            break;         // green on top
        case AxNegZ: cubeW->moveBackLayerPrime();        break;         // blue on top
        case AxPosX: cubeW->moveRightLayerPrime();       break;         // red on top
        case AxNegX: cubeW->moveLeftLayer();             break;         // orange on top
        }
        cubeW->update();
        };

    auto doUpPressed = [cubeW, this] {
        Axis top = detectTopAxis(cubeW->xRotation(), cubeW->yRotation());
        switch (top) {
        case AxPosY: cubeW->moveUpLayer();               break;         // white on top
        case AxNegY: cubeW->moveDownLayerPrime();        break;         // yellow on top
        case AxPosZ: cubeW->moveFrontLayer();            break;         // green on top
        case AxNegZ: cubeW->moveBackLayerPrime();        break;         // blue on top
        case AxPosX: cubeW->moveRightLayerPrime();       break;         // red on top
        case AxNegX: cubeW->moveLeftLayer();             break;         // orange on top
        }
        cubeW->update();
        };

    // addButton utility
    auto addBtn = [&](const QString& text, const std::function<void()>& fn, int r, int c) {
        auto* b = new QPushButton(text, cWidget);
        btnGrid->addWidget(b, r, c);
        connect(b, &QPushButton::clicked, fn);
        };

    // Notations buttons
    addBtn("U", [cubeW] { cubeW->moveUpLayer(); cubeW->update(); }, 0, 0);
    addBtn("U'", [cubeW] { cubeW->moveUpLayerPrime(); cubeW->update(); }, 0, 1);
    addBtn("R", [cubeW] { cubeW->moveRightLayer();  cubeW->update(); }, 1, 0);
    addBtn("R'", [cubeW] { cubeW->moveRightLayerPrime(); cubeW->update(); }, 1, 1);
    addBtn("F", [cubeW] { cubeW->moveFrontLayer();  cubeW->update(); }, 2, 0);
    addBtn("F'", [cubeW] { cubeW->moveFrontLayerPrime(); cubeW->update(); }, 2, 1);
    addBtn("D", [cubeW] { cubeW->moveDownLayer();  cubeW->update(); }, 3, 0);
    addBtn("D'", [cubeW] { cubeW->moveDownLayerPrime(); cubeW->update(); }, 3, 1);
    addBtn("L", [cubeW] { cubeW->moveLeftLayer();  cubeW->update(); }, 4, 0);
    addBtn("L'", [cubeW] { cubeW->moveLeftLayerPrime(); cubeW->update(); }, 4, 1);
    addBtn("B", [cubeW] { cubeW->moveBackLayer();  cubeW->update(); }, 5, 0);
    addBtn("B'", [cubeW] { cubeW->moveBackLayerPrime(); cubeW->update(); }, 5, 1);
    addBtn("Scramble Cube", [cubeW,this] { scrambleCube(*cubeW); cubeW->update(); }, 6, 0);
    addBtn("Reset Cube", [cubeW] { cubeW->resetCube(); cubeW->update(); }, 6, 1);

    startTimerBtn = new QPushButton("Start Timer", this);
    stopTimerBtn = new QPushButton("Stop Timer", this);

    btnGrid->addWidget(startTimerBtn, 7, 0);
    btnGrid->addWidget(stopTimerBtn, 7, 1);

    connect(startTimerBtn, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(stopTimerBtn, &QPushButton::clicked, this, &MainWindow::stopTimer);

    rootLay->addWidget(cubeW, 1);
    rootLay->addLayout(btnGrid);
    cw->setCentralWidget(cWidget);
    cw->resize(900, 650);
    cw->show();
}

void MainWindow::openPenaltyDialog(int index) {
    PenaltyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        switch (dialog.selectedPenalty) {
        case PenaltyDialog::Plus2:
            if (solvesVec[index] != solvesVecRawData[index]) {
                break;
            }
            solvesVec[index] += 2.000;
            break;
        case PenaltyDialog::DNF:
            solvesVec[index] = 0.000;
            break;
        case PenaltyDialog::None:
            solvesVec[index] = solvesVecRawData[index];
            break;
        default:
            break;
        }

        rewriteTable();
        updateBestSolve();
        calcCurrentAo5();
        calcBestAo5();
    }
}

void MainWindow::openSettingsDialog() {
    SettingsDialog dialog(m_timerValue, m_backgroundColor, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_timerValue = dialog.getBufferTime();
        m_backgroundColor = dialog.getSelectedColor();

        QPalette pal = this->palette();
        pal.setColor(QPalette::Window, m_backgroundColor);
        this->setAutoFillBackground(true);
        this->setPalette(pal);
        this->update();
    }
}

QString MainWindow::generateScramble(int length) {
    static const QVector<QString> moves = {
        "U","U'","U2","D","D'","D2",
        "L","L'","L2","R","R'","R2",
        "F","F'","F2","B","B'","B2"
    };

    auto axisOf = [&](const QString& m) {
        switch (m.at(0).toLatin1()) {
        case 'U': case 'D': return 0;
        case 'L': case 'R': return 1;
        default:            return 2;
        }
        };

    QRandomGenerator rng = QRandomGenerator::securelySeeded();
    QStringList scr;
    int lastAxis = -1;
    for (int i = 0; i < length; ++i) {
        int idx, ax;
        do {
            idx = rng.bounded(moves.size());
            ax = axisOf(moves[idx]);
        } while (ax == lastAxis);
        scr << moves[idx];
        lastAxis = ax;
    }
    return scr.join(' ');
}

void MainWindow::onUpdateTimer()
{
    double secs = m_elapsed.elapsed() / 1000.0;
    m_timerLabel->setText(QString::asprintf("%.3f", secs));
}

void MainWindow::scrambleCube(CubeGLWidget& cube) {
    QStringList currentScramble = m_scrambleLabel->text().split(' ', Qt::SkipEmptyParts);

    static const std::unordered_map<QString, std::function<void(CubeGLWidget&)>> moveIndex {
        { "U", &CubeGLWidget::moveUpLayer },
        { "U'", &CubeGLWidget::moveUpLayerPrime },
        { "U2", &CubeGLWidget::moveUpLayer2 },
        { "D", &CubeGLWidget::moveDownLayer },
        { "D'", &CubeGLWidget::moveDownLayerPrime },
        { "D2", &CubeGLWidget::moveDownLayer2 },
        { "R", &CubeGLWidget::moveRightLayer },
        { "R'", &CubeGLWidget::moveRightLayerPrime },
        { "R2", &CubeGLWidget::moveRightLayer2 },
        { "L", &CubeGLWidget::moveLeftLayer },
        { "L'", &CubeGLWidget::moveLeftLayerPrime },
        { "L2", &CubeGLWidget::moveLeftLayer2 },
        { "F", &CubeGLWidget::moveFrontLayer },
        { "F'", &CubeGLWidget::moveFrontLayerPrime },
        { "F2", &CubeGLWidget::moveFrontLayer2 },
        { "B", &CubeGLWidget::moveBackLayer },
        { "B'", &CubeGLWidget::moveBackLayerPrime },
        { "B2", &CubeGLWidget::moveBackLayer2 }
    };

    for (const QString& move : currentScramble) {
        
        auto it = moveIndex.find(move);

        if (it != moveIndex.end()) {
            it->second(cube);
        }
        else {
            break;
        }        
    }
}

void MainWindow::prevScramble() {
    if (m_currentScrambleIndex > 0) {
        m_currentScrambleIndex--;
        m_scrambleLabel->setText(m_scramblesVec[m_currentScrambleIndex]);
    }
}

void MainWindow::nextScramble() {
    if (m_currentScrambleIndex < m_scramblesVec.size() - 1) {
        m_currentScrambleIndex++;
        m_scrambleLabel->setText(m_scramblesVec[m_currentScrambleIndex]);
    }
    else {
        QString newScramble = generateScramble(20);
        m_scramblesVec.push_back(newScramble);
        m_scrambleLabel->setText(newScramble);
        m_currentScrambleIndex++;
    }
}

void MainWindow::calcBestSolve() {
    if (m_bestSolveTime == 0.000)
    {
        m_bestSolveTime = m_currentSolveTime;
    }
    else if (m_bestSolveTime != 0.000 && m_currentSolveTime < m_bestSolveTime) {
        m_bestSolveTime = m_currentSolveTime;
    }

    m_bestSolve->setText("Best Solve: " + QString::number(m_bestSolveTime, 'f', 3));
}

void MainWindow::calcCurrentAo5() {
    double fastestTime;
    double slowestTime;
    int fastestIdx = -1;
    int slowestIdx = -1;
    QVector<double> tempVec = solvesVec;

    if (tempVec.size() == 5 || tempVec.size() == 6) {
        if (tempVec.size() == 6) {
            solvesVec.removeAt(0);
            tempVec.removeAt(0);
        }

        int dnfCount = 0;
        for (double time : tempVec)
            if (time == 0.000)
                dnfCount++;

        if (dnfCount >= 2) {
            m_currentAo5Time = 0.000;
            m_currentAo5->setText("Current Ao5: DNF");
            if (m_bestAo5Time == 0.000)
                m_bestAo5->setText("Best Ao5: DNF");
            return;
        }

        fastestTime = std::numeric_limits<double>::max();
        for (int i = 0; i < tempVec.size(); ++i) {
            if (tempVec[i] != 0.000 && tempVec[i] < fastestTime) {
                fastestTime = tempVec[i];
                fastestIdx = i;
            }
        }

        slowestTime = -1.0;
        for (int i = 0; i < tempVec.size(); ++i) {
            if (tempVec[i] == 0.000 || tempVec[i] > slowestTime) {
                slowestTime = tempVec[i];
                slowestIdx = i;
            }
        }

        if (fastestIdx == -1 || slowestIdx == -1 || fastestIdx == slowestIdx) {
            m_currentAo5Time = 0.000;
            m_currentAo5->setText("Current Ao5: DNF");
            if (m_bestAo5Time == 0.000)
                m_bestAo5->setText("Best Ao5: DNF");
            return;
        }

        tempVec.removeAt(std::max(fastestIdx, slowestIdx));
        tempVec.removeAt(std::min(fastestIdx, slowestIdx));

        double sum = 0.0;
        for (double t : tempVec)
            sum += t;

        double average = sum / 3.0;
        m_currentAo5Time = average;
        m_currentAo5->setText("Current Ao5: " + QString::number(average, 'f', 3));

        calcBestAo5();
    }
}

void MainWindow::calcBestAo5() {
    if (m_bestAo5Time == 0.000)
    {
        m_bestAo5Time = m_currentAo5Time;
    }
    else if (m_bestAo5Time != 0.000 && m_currentAo5Time < m_bestAo5Time) {
        m_bestAo5Time = m_currentAo5Time;
    }

    m_bestAo5->setText("Best Ao5: " + QString::number(m_bestAo5Time, 'f', 3));
}

void MainWindow::rewriteTable() {
    m_solvesTable->setRowCount(0); 

    for (int i = 0; i < solvesVec.size(); ++i) {
        m_solvesTable->insertRow(i);

        auto* idxItem = new QTableWidgetItem(QString::number(i + 1));
        idxItem->setTextAlignment(Qt::AlignCenter);
        m_solvesTable->setItem(i, 0, idxItem);

        QString displayTime = (solvesVec[i] == 0.000) ? "DNF" : QString::asprintf("%.3f", solvesVec[i]);
        auto* timeItem = new QTableWidgetItem(displayTime);
        timeItem->setTextAlignment(Qt::AlignCenter);
        m_solvesTable->setItem(i, 1, timeItem);

        QPushButton* penaltyButton = new QPushButton("Penalty");
        m_solvesTable->setCellWidget(i, 2, penaltyButton);

        connect(penaltyButton, &QPushButton::clicked, this, [this, i]() {
            openPenaltyDialog(i);
            });
    }
}

void MainWindow::updateBestSolve() {
    double currentBestSolve = solvesVec[0];

    for (int i = 0; i < solvesVec.size(); i++) {
        if (solvesVec[i] < currentBestSolve && solvesVec[i] != 0.000) {
            currentBestSolve = solvesVec[i];
        }
    }

    m_bestSolveTime = currentBestSolve;
    m_bestSolve->setText("Best Solve: " + QString::number(m_bestSolveTime, 'f', 3));
}

void MainWindow::updateCurrentAo5() {

}

void MainWindow::updateBestAo5() {

}

void MainWindow::clearSolvesList() {
    solvesVec.clear();
    solvesVecRawData.clear();
    rewriteTable();
    m_timerLabel->setText("0.000");
}

void MainWindow::startTimer() {
    if (!m_running) {
        m_elapsed.restart();
        m_update->start();
        m_running = true;
        m_timerLabel->setText("0.000");
        m_timerLabel->setStyleSheet("font-size: 80px; font-weight: bold;");
        m_instructionLabel->hide();
    }
}

void MainWindow::stopTimer() {
    if (m_running) {

        m_update->stop();
        m_running = false;
        double secs = m_elapsed.elapsed() / 1000.0;
        m_currentSolveTime = secs;
        solvesVec.push_back(secs);
        solvesVecRawData.push_back(secs);

        rewriteTable();

        m_timerLabel->setText(QString::asprintf("%.3f s", secs));

        if (m_currentScrambleIndex < m_scramblesVec.size() - 1)
            m_scramblesVec.resize(m_currentScrambleIndex + 1);

        QString newScramble = generateScramble(20);
        m_scramblesVec.push_back(newScramble);
        m_currentScrambleIndex = m_scramblesVec.size() - 1;
        m_scrambleLabel->setText(newScramble);

        calcBestSolve();
        calcCurrentAo5();
        calcBestAo5();
    }
}