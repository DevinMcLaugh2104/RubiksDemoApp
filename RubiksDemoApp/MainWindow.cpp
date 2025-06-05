#include "mainwindow.h"
#include "CubeWindow.h"
#include "SettingsDialog.h"
#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QRandomGenerator>
#include <QPushButton>          

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_update(new QTimer(this)), m_running(false), m_holdActive(false)
{
    // catch all key events
    qApp->installEventFilter(this);

    // scramble label
    m_scrambleLabel = new QLabel(this);
    m_scrambleLabel->setAlignment(Qt::AlignCenter);
    m_scrambleLabel->setWordWrap(true);
    m_scrambleLabel->setText(generateScramble(20));
    m_scrambleLabel->setSizeIncrement(200, 200);

    // timer display
    m_label = new QLabel("Hold Space ≥1 s, then release to start", this);
    m_label->setAlignment(Qt::AlignCenter);
    auto* timerBox = new QGroupBox("Timer", this);
    auto* timerLayout = new QVBoxLayout;
    timerLayout->addWidget(m_label);
    timerLayout->addStretch();
    timerBox->setLayout(timerLayout);

    // results table
    m_table = new QTableWidget(this);
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({ "#", "Time (s)" });
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);

    // overall layout
    auto* central = new QWidget(this);
    auto* outerLayout = new QVBoxLayout;
    auto* contentLayout = new QHBoxLayout;
    contentLayout->addWidget(timerBox, 0);
    contentLayout->addWidget(m_table, 1);
    outerLayout->addWidget(m_scrambleLabel);
    outerLayout->addLayout(contentLayout);

    // --- Add the “Open Cube” button ----------
    cubeButton = new QPushButton("Open Cube", this);
    outerLayout->addWidget(cubeButton);
    connect(cubeButton, &QPushButton::clicked,
        this, &MainWindow::onShowCube);

    central->setLayout(outerLayout);
    setCentralWidget(central);

    // live‐update hookup
    m_update->setInterval(50);
    connect(m_update, &QTimer::timeout, this, &MainWindow::onUpdateTimer);
    // --- Add the "Settings" button -----------
    settingsButton = new QPushButton("Settings", this);
    outerLayout->addWidget(settingsButton);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsDialog);
}

MainWindow::~MainWindow() = default;

void MainWindow::onUpdateTimer()
{
    double secs = m_elapsed.elapsed() / 1000.0;
    m_label->setText(QString::asprintf("%.3f s", secs));
}

bool MainWindow::eventFilter(QObject*, QEvent* event)
{
    // only handle non-auto-repeat Space
    if ((event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) &&
        static_cast<QKeyEvent*>(event)->isAutoRepeat())
    {
        return false;
    }

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
                    m_label->setText("0.000 s");
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

                m_label->setText("Final: " + timeItem->text() + " s");
                m_scrambleLabel->setText(generateScramble(20));
            }

            m_holdActive = false;
            return true;
        }
    }

    return QMainWindow::eventFilter(qApp, event);
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

void MainWindow::onShowCube()
{
    CubeWindow* cw = new CubeWindow(this);
    cw->setAttribute(Qt::WA_DeleteOnClose);
    cw->show();
}
void MainWindow::openSettingsDialog() {
    SettingsDialog dialog(m_timerValue, this);  
    if (dialog.exec() == QDialog::Accepted) {
        m_timerValue = dialog.getBufferTime();  
    }
}
