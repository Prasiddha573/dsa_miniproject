#include "minesweeper.h"
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

/**
 * @brief Constructs the MainWindow with a professional dark theme and fixed size
 * @param parent The parent widget (default nullptr)
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_mineStatBtn(nullptr),
    m_timerStatBtn(nullptr),
    m_bestStatBtn(nullptr),
    backButton(nullptr),
    flagsPlaced(0),
    gamesPlayed(0),
    gamesWon(0),
    secondsElapsed(0),
    bestTime(0)
{
    // Set fixed window size for consistent UI experience
    setFixedSize(1000, 850);

    // Apply global dark theme styling
    applyGlobalStyle();

    // Initialize Central Widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setStyleSheet("#centralWidget { background-color: " + ThemeColors::DARK_BG + "; }");

    // Main Vertical Layout
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ==================== Title Section ====================
    titleLabel = new QLabel("MINESWEEPER");
    titleLabel->setFont(QFont("Segoe UI", 28, QFont::Bold));
    titleLabel->setStyleSheet("color: " + ThemeColors::PRIMARY + ";");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ==================== Stats Bar ====================
    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(15);
    statsLayout->setContentsMargins(0, 10, 0, 10);

    QString statButtonStyle =
        "QPushButton {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   min-width: 100px;"
        "   text-align: left;"
        "}";

    // Mine Counter Button
    m_mineStatBtn = new QPushButton("💣  0");
    m_mineStatBtn->setFont(QFont("Segoe UI", 14, QFont::Bold));
    m_mineStatBtn->setStyleSheet(statButtonStyle +
                                 "QPushButton { background-color: " + ThemeColors::ERROR + "; color: white; }"
                                                                                           "QPushButton:hover { background-color: #ff9aa8; }"
                                 );
    m_mineStatBtn->setFixedHeight(50);
    m_mineStatBtn->setCursor(Qt::ArrowCursor);
    m_mineStatBtn->setEnabled(false);

    // Timer Button
    m_timerStatBtn = new QPushButton("⏱️  0");
    m_timerStatBtn->setFont(QFont("Segoe UI", 14, QFont::Bold));
    m_timerStatBtn->setStyleSheet(statButtonStyle +
                                  "QPushButton { background-color: " + ThemeColors::PRIMARY + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                                   "QPushButton:hover { background-color: #9bb5f9; }"
                                  );
    m_timerStatBtn->setFixedHeight(50);
    m_timerStatBtn->setCursor(Qt::ArrowCursor);
    m_timerStatBtn->setEnabled(false);

    // Best Score Button
    m_bestStatBtn = new QPushButton("🏆  0");
    m_bestStatBtn->setFont(QFont("Segoe UI", 14, QFont::Bold));
    m_bestStatBtn->setStyleSheet(statButtonStyle +
                                 "QPushButton { background-color: " + ThemeColors::WARNING + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                                  "QPushButton:hover { background-color: #ffb86b; }"
                                 );
    m_bestStatBtn->setFixedHeight(50);
    m_bestStatBtn->setCursor(Qt::ArrowCursor);
    m_bestStatBtn->setEnabled(false);

    // Restart Button
    restartButton = new QPushButton("↻  RESTART");
    restartButton->setFont(QFont("Segoe UI", 14, QFont::Bold));
    restartButton->setStyleSheet(statButtonStyle +
                                 "QPushButton { background-color: " + ThemeColors::ACCENT + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                                 "QPushButton:hover { background-color: #d5b4ff; }"
                                 );
    restartButton->setFixedHeight(50);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::initializeGame);

    // Back Button
    backButton = new QPushButton("←  BACK");
    backButton->setFont(QFont("Segoe UI", 14, QFont::Bold));
    backButton->setStyleSheet(statButtonStyle +
                              "QPushButton { background-color: " + ThemeColors::BORDER + "; color: " + ThemeColors::TEXT + "; }"
                                                                                                                           "QPushButton:hover { background-color: " + ThemeColors::HOVER + "; }"
                              );
    backButton->setFixedHeight(50);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToMainClicked);

    statsLayout->addStretch();
    statsLayout->addWidget(m_mineStatBtn);
    statsLayout->addWidget(m_timerStatBtn);
    statsLayout->addWidget(m_bestStatBtn);
    statsLayout->addWidget(restartButton);
    statsLayout->addWidget(backButton);
    statsLayout->addStretch();

    mainLayout->addLayout(statsLayout);

    // ==================== Difficulty Selection ====================
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->setSpacing(15);

    easyBtn = new QPushButton("EASY");
    mediumBtn = new QPushButton("MEDIUM");
    hardBtn = new QPushButton("HARD");

    QString baseButtonStyle =
        "QPushButton {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px 25px;"
        "   min-width: 100px;"
        "}";

    easyBtn->setStyleSheet(baseButtonStyle +
                           "QPushButton { background-color: " + ThemeColors::SUCCESS + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                            "QPushButton:hover { background-color: #b4f9a4; }"
                                                                                                                            "QPushButton:checked { border: 2px solid white; }"
                           );

    mediumBtn->setStyleSheet(baseButtonStyle +
                             "QPushButton { background-color: " + ThemeColors::WARNING + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                              "QPushButton:hover { background-color: #ffb86b; }"
                                                                                                                              "QPushButton:checked { border: 2px solid white; }"
                             );

    hardBtn->setStyleSheet(baseButtonStyle +
                           "QPushButton { background-color: " + ThemeColors::ERROR + "; color: white; }"
                                                                                     "QPushButton:hover { background-color: #ff9aa8; }"
                                                                                     "QPushButton:checked { border: 2px solid white; }"
                           );

    easyBtn->setCheckable(true);
    mediumBtn->setCheckable(true);
    hardBtn->setCheckable(true);

    connect(easyBtn, &QPushButton::clicked, this, &MainWindow::setEasy);
    connect(mediumBtn, &QPushButton::clicked, this, &MainWindow::setMedium);
    connect(hardBtn, &QPushButton::clicked, this, &MainWindow::setHard);

    difficultyLayout->addStretch();
    difficultyLayout->addWidget(easyBtn);
    difficultyLayout->addWidget(mediumBtn);
    difficultyLayout->addWidget(hardBtn);
    difficultyLayout->addStretch();

    mainLayout->addLayout(difficultyLayout);
    mainLayout->addSpacing(10);

    // ==================== Game Grid Container ====================
    QWidget *gridContainerWidget = new QWidget();
    gridContainerWidget->setStyleSheet("background-color: transparent;");

    gridLayout = new QGridLayout(gridContainerWidget);
    gridLayout->setSpacing(4);
    gridLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(gridContainerWidget, 0, Qt::AlignCenter);

    // ==================== Bottom Action Buttons ====================
    bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(20);

    hintBtn = new QPushButton("💡 HINT");
    undoBtn = new QPushButton("↩ UNDO");
    statsBtn = new QPushButton("📊 STATS");

    QString actionButtonStyle =
        "QPushButton {"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 10px 25px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   min-width: 100px;"
        "}";

    hintBtn->setStyleSheet(actionButtonStyle +
                           "QPushButton { background-color: " + ThemeColors::PRIMARY + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                            "QPushButton:hover { background-color: #9bb5f9; }"
                           );

    undoBtn->setStyleSheet(actionButtonStyle +
                           "QPushButton { background-color: " + ThemeColors::SECONDARY + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                              "QPushButton:hover { background-color: #b4f9f9; }"
                           );

    statsBtn->setStyleSheet(actionButtonStyle +
                            "QPushButton { background-color: " + ThemeColors::ACCENT + "; color: " + ThemeColors::DARK_BG + "; }"
                                                                                                                            "QPushButton:hover { background-color: #d5b4ff; }"
                            );

    connect(hintBtn, &QPushButton::clicked, this, &MainWindow::giveHint);
    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::undoLastMove);
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::showStatistics);

    bottomLayout->addStretch();
    bottomLayout->addWidget(hintBtn);
    bottomLayout->addWidget(undoBtn);
    bottomLayout->addWidget(statsBtn);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);
    mainLayout->addSpacing(10);

    // ==================== Timer Initialization ====================
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    // Load saved data
    loadBestScore();
    loadStatistics();

    // Start with Easy difficulty
    setupDifficulty(EASY);
    easyBtn->setChecked(true);
    initializeGame();

    // Shadow effects removed to prevent painter conflicts
}

/**
 * @brief Public method to start/reset the game
 */
void MainWindow::startNewGame()
{
    setupDifficulty(EASY);
    easyBtn->setChecked(true);
    mediumBtn->setChecked(false);
    hardBtn->setChecked(false);
    initializeGame();
}

/**
 * @brief Set difficulty and start new game
 * @param diff Difficulty level to set
 */
void MainWindow::setDifficultyAndStart(Difficulty diff)
{
    setupDifficulty(diff);
    easyBtn->setChecked(diff == EASY);
    mediumBtn->setChecked(diff == MEDIUM);
    hardBtn->setChecked(diff == HARD);
    initializeGame();
}

/**
 * @brief Handles back button click
 */
void MainWindow::onBackToMainClicked()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }
    emit backToMainHome();
}

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    if (timer) {
        timer->stop();
        delete timer;
    }
}

/**
 * @brief Applies global dark theme styling
 */
void MainWindow::applyGlobalStyle()
{
    setStyleSheet(
        "QMainWindow { background-color: " + ThemeColors::DARK_BG + "; }"
                                                                    "QToolTip { background-color: " + ThemeColors::SURFACE + "; color: " + ThemeColors::TEXT + "; border: 1px solid " + ThemeColors::BORDER + "; }"
                                                                                                                  "QMessageBox { background-color: " + ThemeColors::DARK_BG + "; }"
                                 "QMessageBox QLabel { color: " + ThemeColors::TEXT + "; }"
                              "QMessageBox QPushButton { background-color: " + ThemeColors::SURFACE + "; color: " + ThemeColors::TEXT +
        "; border: 1px solid " + ThemeColors::BORDER + "; border-radius: 5px; padding: 5px 15px; }"
                                                       "QMessageBox QPushButton:hover { background-color: " + ThemeColors::HOVER + "; }"
        );
}

/**
 * @brief Styles action buttons
 */
void MainWindow::styleActionButton(QPushButton *btn, const QString &color)
{
    btn->setFixedHeight(45);
    btn->setFont(QFont("Segoe UI", 12, QFont::Bold));
    btn->setStyleSheet(
        "QPushButton { background-color: " + color + "; color: " + ThemeColors::DARK_BG +
        "; border: none; border-radius: 8px; padding: 8px 20px; }"
        "QPushButton:hover { background-color: " + color + "; opacity: 0.8; }"
                  "QPushButton:pressed { background-color: " + color + "; opacity: 0.6; }"
        );
}

/**
 * @brief Placeholder for button styling
 */
void MainWindow::styleButton(QPushButton* btn)
{
    if (!btn) return;
}

/**
 * @brief Creates a separator line
 */
QFrame* MainWindow::createSeparator()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("QFrame { border: 1px solid " + ThemeColors::BORDER + "; }");
    return line;
}

/**
 * @brief Sets up game parameters based on difficulty
 */
void MainWindow::setupDifficulty(Difficulty diff)
{
    difficulty = diff;
    easyBtn->setChecked(diff == EASY);
    mediumBtn->setChecked(diff == MEDIUM);
    hardBtn->setChecked(diff == HARD);

    switch(diff) {
    case EASY:
        rows = 6;
        cols = 6;
        mineCount = 6;
        break;
    case MEDIUM:
        rows = 10;
        cols = 10;
        mineCount = 15;
        break;
    case HARD:
        rows = 14;
        cols = 14;
        mineCount = 30;
        break;
    }
}

/**
 * @brief Initializes a new game
 */
void MainWindow::initializeGame()
{
    timer->stop();
    secondsElapsed = 0;
    flagsPlaced = 0;

    m_timerStatBtn->setText("⏱️  0");
    m_mineStatBtn->setText("💣  " + QString::number(mineCount));
    m_bestStatBtn->setText("🏆  " + (bestTime > 0 ? QString::number(bestTime) : "0"));

    restartButton->setText("↻  RESTART");
    moveHistory.clear();

    // Clear existing grid
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    buttons.clear();
    board.clear();
    revealed.clear();
    flagged.clear();

    buttons.resize(rows);
    board.resize(rows);
    revealed.resize(rows);
    flagged.resize(rows);

    int buttonSize;
    if (rows <= 6) buttonSize = 70;
    else if (rows <= 10) buttonSize = 55;
    else buttonSize = 45;

    for(int r = 0; r < rows; r++) {
        buttons[r].resize(cols);
        board[r].resize(cols);
        revealed[r].resize(cols);
        flagged[r].resize(cols);

        for(int c = 0; c < cols; c++) {
            board[r][c] = 0;
            revealed[r][c] = false;
            flagged[r][c] = false;
        }

        for(int c = 0; c < cols; c++) {
            QPushButton *btn = new QPushButton("");
            btn->setFixedSize(buttonSize, buttonSize);

            int fontSize = buttonSize >= 70 ? 18 : (buttonSize >= 55 ? 16 : 14);
            btn->setFont(QFont("Segoe UI", fontSize, QFont::Bold));

            btn->setStyleSheet(
                "QPushButton {"
                "   background-color: " + ThemeColors::SURFACE + ";"
                                         "   border: 1px solid " + ThemeColors::BORDER + ";"
                                        "   border-radius: " + QString::number(buttonSize/8) + "px;"
                                                    "   color: " + ThemeColors::TEXT + ";"
                                      "}"
                                      "QPushButton:hover {"
                                      "   background-color: " + ThemeColors::HOVER + ";"
                                       "   border-color: " + ThemeColors::PRIMARY + ";"
                                         "}"
                                         "QPushButton:disabled {"
                                         "   background-color: " + ThemeColors::DARKER_BG + ";"
                                           "   border-color: " + ThemeColors::BORDER + ";"
                                        "}"
                );

            // Individual button shadows removed to prevent painter conflicts

            gridLayout->addWidget(btn, r, c);
            buttons[r][c] = btn;

            connect(btn, &QPushButton::clicked, this, &MainWindow::handleCellClick);
            btn->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(btn, &QPushButton::customContextMenuRequested, this, &MainWindow::handleCellRightClick);
        }
    }

    placeMines();
    calculateNumbers();
    timer->start(1000);
}

/**
 * @brief Places mines randomly on the board
 */
void MainWindow::placeMines()
{
    int placed = 0;
    while(placed < mineCount) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);
        if(board[r][c] != -1) {
            board[r][c] = -1;
            placed++;
        }
    }
}

/**
 * @brief Calculates adjacent mine counts
 */
void MainWindow::calculateNumbers()
{
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(board[r][c] == -1) continue;

            int count = 0;
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    int nr = r + i, nc = c + j;
                    if(nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == -1)
                        count++;
                }
            }
            board[r][c] = count;
        }
    }
}

/**
 * @brief Handles left-click on a cell
 */
void MainWindow::handleCellClick()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    animateButton(btn);

    int r = -1, c = -1;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(buttons[i][j] == btn) {
                r = i;
                c = j;
                break;
            }
        }
    }

    if(r == -1 || flagged[r][c] || revealed[r][c]) return;

    revealCell(r, c);
    Move move;
    move.r = r;
    move.c = c;
    moveHistory.append(move);
    checkWin();
}

/**
 * @brief Handles right-click (flagging) on a cell
 */
void MainWindow::handleCellRightClick(const QPoint &)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    animateButton(btn);

    int r = -1, c = -1;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(buttons[i][j] == btn) {
                r = i;
                c = j;
                break;
            }
        }
    }

    if(r == -1 || revealed[r][c]) return;

    if(flagged[r][c]) {
        flagged[r][c] = false;
        flagsPlaced--;
        btn->setText("");
        btn->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::SURFACE +
            "; border: 1px solid " + ThemeColors::BORDER +
            "; border-radius: " + QString::number(btn->width()/8) + "px;"
                                                                      " color: " + ThemeColors::TEXT + "; }"
                                  "QPushButton:hover { background-color: " + ThemeColors::HOVER + "; border-color: " + ThemeColors::PRIMARY + "; }"
            );
    } else {
        flagged[r][c] = true;
        flagsPlaced++;
        btn->setText("🚩");
        btn->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::SURFACE +
            "; border: 1px solid " + ThemeColors::ERROR +
            "; border-radius: " + QString::number(btn->width()/8) + "px;"
                                                                      " color: " + ThemeColors::ERROR + "; }"
                                   "QPushButton:hover { background-color: " + ThemeColors::HOVER + "; }"
            );
    }

    m_mineStatBtn->setText("💣  " + QString::number(mineCount - flagsPlaced));
}

/**
 * @brief Reveals a cell
 */
void MainWindow::revealCell(int r, int c)
{
    if(r < 0 || r >= rows || c < 0 || c >= cols) return;
    if(revealed[r][c]) return;
    if(flagged[r][c]) return;

    revealed[r][c] = true;

    if(board[r][c] == -1) {
        buttons[r][c]->setText("💣");
        buttons[r][c]->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::ERROR +
            "; border: 1px solid " + ThemeColors::ERROR +
            "; border-radius: " + QString::number(buttons[r][c]->width()/8) + "px;"
                                                                                " color: white; }"
            );
        restartButton->setText("😵  RESTART");
        gameOver(false);
        return;
    }

    if(board[r][c] > 0) {
        QString numberColor;
        switch(board[r][c]) {
        case 1: numberColor = "#89b4fa"; break;
        case 2: numberColor = "#a6e3a1"; break;
        case 3: numberColor = "#f38ba8"; break;
        case 4: numberColor = "#cba6f7"; break;
        case 5: numberColor = "#fab387"; break;
        default: numberColor = "#94e2d5"; break;
        }

        buttons[r][c]->setText(QString::number(board[r][c]));
        buttons[r][c]->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::DARKER_BG +
            "; border: 1px solid " + ThemeColors::BORDER +
            "; border-radius: " + QString::number(buttons[r][c]->width()/8) + "px;"
                                                                                " color: " + numberColor + "; }"
            );
    } else {
        buttons[r][c]->setText("");
        buttons[r][c]->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::DARKER_BG +
            "; border: 1px solid " + ThemeColors::BORDER +
            "; border-radius: " + QString::number(buttons[r][c]->width()/8) + "px;"
                                                                                " color: " + ThemeColors::TEXT + "; }"
            );

        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                int nr = r + i, nc = c + j;
                if(nr >= 0 && nr < rows && nc >= 0 && nc < cols && !flagged[nr][nc] && !revealed[nr][nc])
                    revealCell(nr, nc);
            }
        }
    }

    buttons[r][c]->setEnabled(false);
}

/**
 * @brief Checks if player has won
 */
void MainWindow::checkWin()
{
    int revealedCount = 0;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(revealed[r][c]) revealedCount++;
        }
    }

    if(revealedCount == (rows * cols - mineCount)) {
        timer->stop();
        restartButton->setText("🏆  RESTART");

        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                if(board[r][c] == -1 && !flagged[r][c])
                    buttons[r][c]->setText("🚩");
            }
        }

        saveBestScore(secondsElapsed);
        saveStatistics(true);

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Victory!");
        msgBox.setText(QString("🎉 Congratulations! You won in %1 seconds! 🎉").arg(secondsElapsed));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: " + ThemeColors::DARK_BG + "; }"
                                                                        "QLabel { color: " + ThemeColors::SUCCESS + "; font-size: 14px; }"
            );
        msgBox.exec();
    }
}

/**
 * @brief Handles game over
 */
void MainWindow::gameOver(bool win)
{
    timer->stop();
    saveStatistics(false);

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            buttons[r][c]->setEnabled(false);
            if(board[r][c] == -1 && !flagged[r][c])
                buttons[r][c]->setText("💣");
        }
    }

    if(!win) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Game Over");
        msgBox.setText("💥 You hit a mine! Better luck next time! 💥");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: " + ThemeColors::DARK_BG + "; }"
                                                                        "QLabel { color: " + ThemeColors::ERROR + "; font-size: 14px; }"
            );
        msgBox.exec();
    }
}

/**
 * @brief Provides a hint
 */
void MainWindow::giveHint()
{
    int r, c;
    if(safeCell(r, c)) {
        QString originalStyle = buttons[r][c]->styleSheet();

        buttons[r][c]->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::WARNING +
            "; border: 2px solid " + ThemeColors::WARNING +
            "; border-radius: " + QString::number(buttons[r][c]->width()/8) + "px;"
                                                                                " color: " + ThemeColors::DARK_BG + "; }"
            );

        QTimer::singleShot(500, [this, r, c, originalStyle]() {
            if(r >= 0 && r < rows && c >= 0 && c < cols) {
                if(!revealed[r][c] && !flagged[r][c]) {
                    buttons[r][c]->setStyleSheet(originalStyle);
                }
            }
        });
    }
}

/**
 * @brief Finds a safe cell
 */
bool MainWindow::safeCell(int &r, int &c)
{
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(!revealed[i][j] && !flagged[i][j] && board[i][j] != -1) {
                r = i;
                c = j;
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Undoes last move
 */
void MainWindow::undoLastMove()
{
    if(moveHistory.isEmpty()) return;
    Move last = moveHistory.takeLast();

    if(last.r >= 0 && last.r < rows && last.c >= 0 && last.c < cols) {
        revealed[last.r][last.c] = false;
        buttons[last.r][last.c]->setEnabled(true);
        buttons[last.r][last.c]->setText("");
        buttons[last.r][last.c]->setStyleSheet(
            "QPushButton { background-color: " + ThemeColors::SURFACE +
            "; border: 1px solid " + ThemeColors::BORDER +
            "; border-radius: " + QString::number(buttons[last.r][last.c]->width()/8) + "px;"
                                                                                          " color: " + ThemeColors::TEXT + "; }"
                                  "QPushButton:hover { background-color: " + ThemeColors::HOVER + "; border-color: " + ThemeColors::PRIMARY + "; }"
            );
    }
}

/**
 * @brief Animates button press
 */
void MainWindow::animateButton(QPushButton* btn)
{
    if (!btn) return;

    QPropertyAnimation *animation = new QPropertyAnimation(btn, "geometry");
    animation->setDuration(100);
    animation->setStartValue(btn->geometry());
    QRect endRect = btn->geometry();
    endRect.setSize(endRect.size() * 0.95);
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

/**
 * @brief Updates timer display
 */
void MainWindow::updateTimer()
{
    secondsElapsed++;
    m_timerStatBtn->setText("⏱️  " + QString::number(secondsElapsed));
}

/**
 * @brief Loads best score from file
 */
void MainWindow::loadBestScore()
{
    QFile file(scoreFile);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in >> bestTime;
        file.close();
    } else {
        bestTime = 0;
    }
}

/**
 * @brief Saves best score to file
 */
void MainWindow::saveBestScore(int time)
{
    if(bestTime == 0 || time < bestTime) {
        bestTime = time;
        m_bestStatBtn->setText("🏆  " + QString::number(bestTime));

        QFile file(scoreFile);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out << bestTime;
            file.close();
        }
    }
}

/**
 * @brief Loads statistics from file
 */
void MainWindow::loadStatistics()
{
    QFile file(statsFile);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in >> gamesPlayed >> gamesWon;
        file.close();
    }
}

/**
 * @brief Saves statistics to file
 */
void MainWindow::saveStatistics(bool won)
{
    gamesPlayed++;
    if(won) gamesWon++;

    QFile file(statsFile);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << gamesPlayed << " " << gamesWon;
        file.close();
    }
}

/**
 * @brief Shows statistics
 */
void MainWindow::showStatistics()
{
    double winRate = gamesPlayed > 0 ? (double(gamesWon) / gamesPlayed) * 100 : 0;

    QString statsText = QString(
                            "📊 Statistics\n\n"
                            "Games Played: %1\n"
                            "Games Won: %2\n"
                            "Win Rate: %3%\n"
                            "Best Time: %4 s"
                            ).arg(gamesPlayed).arg(gamesWon).arg(winRate, 0, 'f', 1).arg(bestTime);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Statistics");
    msgBox.setText(statsText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: " + ThemeColors::DARK_BG + "; }"
                                                                    "QLabel { color: " + ThemeColors::TEXT + "; font-size: 14px; }"
        );
    msgBox.exec();
}

/**
 * @brief Updates mine display
 */
void MainWindow::updateMineDisplay()
{
    m_mineStatBtn->setText("💣  " + QString::number(mineCount - flagsPlaced));
}

/**
 * @brief Sets Easy difficulty
 */
void MainWindow::setEasy()
{
    setupDifficulty(EASY);
    initializeGame();
}

/**
 * @brief Sets Medium difficulty
 */
void MainWindow::setMedium()
{
    setupDifficulty(MEDIUM);
    initializeGame();
}

/**
 * @brief Sets Hard difficulty
 */
void MainWindow::setHard()
{
    setupDifficulty(HARD);
    initializeGame();
}
