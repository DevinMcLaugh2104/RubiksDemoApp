#include "mainwindow.h"
#include "CubeWindow.h"
#include "CubeGLWidget.h"
#include "SettingsDialog.h"
#include <QApplication>
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

    // ───── UI top (timer, scramble, table) ──────────────────────────────
    m_scrambleLabel = new QLabel(this);
    m_scrambleLabel->setAlignment(Qt::AlignCenter);
    m_scrambleLabel->setWordWrap(true);
    m_scrambleLabel->setText(generateScramble(20));

    //Scramble Font
    QFont scrambleFont = m_scrambleLabel->font();
    scrambleFont.setPointSize(30);
    m_scrambleLabel->setFont(scrambleFont);

    m_label = new QLabel("0.000", this);
    m_label->setAlignment(Qt::AlignCenter);

    // Timer Font
    QFont timerFont = m_label->font();
    timerFont.setPointSize(60);
    timerFont.setBold(true);
    m_label->setFont(timerFont);

    // Instruction label
    m_instructionLabel = new QLabel("Hold Space ≥1 s, then release to start", this);
    m_instructionLabel->setAlignment(Qt::AlignCenter);
    QFont instrFont = m_instructionLabel->font();
    instrFont.setPointSize(14);
    m_instructionLabel->setFont(instrFont);

    auto* timerBox = new QGroupBox(this);
    auto* timerLay = new QVBoxLayout(timerBox);
    timerLay->addWidget(m_label);
    timerLay->addWidget(m_instructionLabel);
    timerLay->addStretch();
    timerBox->setStyleSheet("border: none;");
    timerBox->setFixedSize(500, 300);
    timerBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({ "#", "Time (s)" });
    m_table->setMaximumWidth(300);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);

    auto* central = new QWidget(this);
    auto* outerLayout = new QVBoxLayout(central);
    auto* contentLay = new QHBoxLayout;
    contentLay->addWidget(m_table, 1);           
    contentLay->addStretch();                    
    contentLay->addWidget(timerBox, 0, Qt::AlignCenter);  
    contentLay->addStretch();                    

    outerLayout->addWidget(m_scrambleLabel);
    outerLayout->addLayout(contentLay);

    // main-window buttons
    cubeButton = new QPushButton("Open Cube", this);
    settingsButton = new QPushButton("Settings", this);
    outerLayout->addWidget(cubeButton);
    outerLayout->addWidget(settingsButton);

    connect(cubeButton, &QPushButton::clicked, this, &MainWindow::onShowCube);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsDialog);

    setCentralWidget(central);

    // timer update
    m_update->setInterval(50);
    connect(m_update, &QTimer::timeout, this, &MainWindow::onUpdateTimer);
}

MainWindow::~MainWindow() = default;

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{

    if ((event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) &&
        static_cast<QKeyEvent*>(event)->isAutoRepeat())
        return false;

    if (event->type() == QEvent::KeyPress)
    {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Space && !m_running && !m_holdActive)
        {
            m_holdActive = true;
            m_holdTimer.restart();
            return true;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        auto* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Space)
        {
            if (!m_running)
            {
                if (m_holdActive && m_holdTimer.elapsed() >= m_timerValue)
                {
                    m_elapsed.restart();
                    m_update->start();
                    m_running = true;
                    m_label->setText("0.000");
                    m_label->setStyleSheet("font-size: 80px; font-weight: bold; ");
                    m_instructionLabel->hide();
                }
            }
            else
            {
                m_update->stop();
                m_running = false;
                double secs = m_elapsed.elapsed() / 1000.0;

                int row = m_table->rowCount();
                m_table->insertRow(row);

                auto* idxItem = new QTableWidgetItem(QString::number(row + 1));
                idxItem->setTextAlignment(Qt::AlignCenter);
                m_table->setItem(row, 0, idxItem);

                auto* timeItem = new QTableWidgetItem(QString::asprintf("%.3f", secs));
                timeItem->setTextAlignment(Qt::AlignCenter);
                m_table->setItem(row, 1, timeItem);

                m_label->setText(timeItem->text() + " s");
                m_scrambleLabel->setText(generateScramble(20));
            }

            m_holdActive = false;
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

// ───── Orientation helper ───────────────────────────────────────────────
// Returns which original cube axis currently points “up” ( +Y in world )
enum Axis { AxPosX, AxNegX, AxPosY, AxNegY, AxPosZ, AxNegZ };

static Axis detectTopAxis(float xDeg, float yDeg)
{
    // build same rotation as view (X then Y), invert to map cube→world
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
    float best = -2;                  // min dot=-1
    for (auto& t : axes) {
        float d = QVector3D::dotProduct(inv.map(t.v), QVector3D(0, 1, 0));
        if (d > best) {
            best = d;
            bestA = t.a;
        }
    }
    return bestA;
}

// ───── show CubeWindow with grid-layout buttons ─────────────────────────
void MainWindow::onShowCube()
{
    auto* cw = new CubeWindow(nullptr);
    cw->setAttribute(Qt::WA_DeleteOnClose);

    // central widget & layouts
    auto* cWidget = new QWidget(cw);
    auto* rootLay = new QVBoxLayout(cWidget);
    auto* btnGrid = new QGridLayout;
    auto* cubeW = new CubeGLWidget(cWidget);

    // helper to map “U” press based on orientation
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

    // U / U'
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

    rootLay->addWidget(cubeW, 1);
    rootLay->addLayout(btnGrid);
    cw->setCentralWidget(cWidget);
    cw->resize(900, 650);
    cw->show();
}

void MainWindow::openSettingsDialog()
{
    SettingsDialog dialog(m_timerValue, m_backgroundColor, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_timerValue = dialog.getBufferTime();
        m_backgroundColor = dialog.getSelectedColor();

        QPalette pal = this->palette();
        pal.setColor(QPalette::Window, m_backgroundColor);
        this->setAutoFillBackground(true);
        this->setPalette(pal);
        this->update();
    }
}

QString MainWindow::generateScramble(int length)
{
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
    for (int i = 0; i < length; ++i)
    {
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
    m_label->setText(QString::asprintf("%.3f", secs));
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


