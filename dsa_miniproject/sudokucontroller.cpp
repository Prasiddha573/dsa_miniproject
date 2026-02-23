#include "sudokucontroller.h"
#include "homescreen.h"
#include "gamescreen.h"
#include "gamesizes.h"
#include <QStackedWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScreen>
#include <QDebug>

SudokuController::SudokuController(QWidget *parent)
    : QMainWindow(parent)
{
    // Set fixed window size for consistent UI experience (like Minesweeper)
    setFixedSize(1100, 750);

    // Center the window on screen
    QRect screenGeo = GameSizes::screenGeometry();
    int x = (screenGeo.width() - width()) / 2;
    int y = (screenGeo.height() - height()) / 2;
    move(x, y);

    setMinimumSize(900, 650); // Ensure minimum size for usability

    stackedWidget = new QStackedWidget(this);

    // Set size policy for stacked widget
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setCentralWidget(stackedWidget);

    // Create screens
    homeScreen = new HomeScreen(this);
    gameScreen = new GameScreen(this);

    // Set size policies for screens
    homeScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    gameScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add screens to stacked widget
    stackedWidget->addWidget(homeScreen);
    stackedWidget->addWidget(gameScreen);

    // Connect signals
    connect(homeScreen, &HomeScreen::startGameClicked, this, &SudokuController::onStartGame);
    connect(homeScreen, &HomeScreen::backToMenuClicked, this, &SudokuController::onBackToMainMenu);
    connect(gameScreen, &GameScreen::backToHomeClicked, this, &SudokuController::onBackToHome);

    // Connect back button to main home
    connect(gameScreen, &GameScreen::backToHomeClicked, this, &SudokuController::backToMainHome);

    setWindowTitle("Sudoku Game");

    // Apply a modern dark theme style to the main window (matching Minesweeper)
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #1a1e2b;"
        "}"
        "QStackedWidget {"
        "   background-color: transparent;"
        "}"
        );

    // Start with home screen
    stackedWidget->setCurrentWidget(homeScreen);

    // Debug: Print current sizes
    GameSizes::debugSizes();
}

SudokuController::~SudokuController()
{
    // Clean up if needed
}

void SudokuController::onStartGame()
{
    QString difficulty = homeScreen->selectedDifficulty();
    gameScreen->startNewGame(difficulty);
    stackedWidget->setCurrentWidget(gameScreen);

    // Ensure game screen has focus for keyboard input
    gameScreen->setFocus();
}

void SudokuController::onBackToMainMenu()
{
    // This goes back to the main application menu (not just home screen)
    emit backToMainHome();
}

void SudokuController::onBackToHome()
{
    stackedWidget->setCurrentWidget(homeScreen);

    // Stop any game timers when going back to home
    gameScreen->stopTimer();

    // Give focus to home screen
    homeScreen->setFocus();
}

void SudokuController::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Update any size-dependent layouts if needed
    if (gameScreen) {
        gameScreen->updateGeometry();
    }
    if (homeScreen) {
        homeScreen->updateGeometry();
    }
}

void SudokuController::keyPressEvent(QKeyEvent *event)
{
    // Handle global keyboard shortcuts
    if (event->key() == Qt::Key_Escape) {
        QWidget *current = stackedWidget->currentWidget();
        if (current == gameScreen) {
            onBackToHome();
        } else if (current == homeScreen) {
            onBackToMainMenu();
        }
    }

    QMainWindow::keyPressEvent(event);
}
