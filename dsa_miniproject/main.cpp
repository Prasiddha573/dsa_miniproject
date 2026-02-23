#include <QApplication>
#include <QStackedWidget>
#include <QDebug>
#include "mainHomeScreen.h"
#include "home.h"
#include "snake.h"
#include "minesweeper.h"
#include "sudokucontroller.h"  // Add Sudoku controller header

/**
 * @brief Main Application Entry Point
 * @param argc Number Of Command Line Arguments
 * @param argv Array Of Command Line Arguments
 * @return Application Exit Code
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set Application-Wide Style
    a.setStyle("Fusion");

    // Create Stacked Widget For Navigation Between Screens
    QStackedWidget stackedWidget;
    stackedWidget.setWindowTitle("Game Hub");
    stackedWidget.setWindowFlags(Qt::FramelessWindowHint);

    // Create All Screens
    MainHomeScreen *mainHome = new MainHomeScreen();
    Home *snakeHome = new Home();
    Snake *snakeGame = new Snake();
    MainWindow *minesweeperGame = new MainWindow();
    SudokuController *sudokuGame = new SudokuController();  // Create Sudoku game

    // Add Screens To Stacked Widget
    int mainHomeIndex = stackedWidget.addWidget(mainHome);
    int snakeHomeIndex = stackedWidget.addWidget(snakeHome);
    int snakeGameIndex = stackedWidget.addWidget(snakeGame);
    int minesweeperIndex = stackedWidget.addWidget(minesweeperGame);
    int sudokuIndex = stackedWidget.addWidget(sudokuGame);  // Add Sudoku to stack

    /**
     * @brief Navigation Flow:
     * MainHomeScreen -> SnakeHome (Difficulty Selection) -> SnakeGame (Actual Game)
     * MainHomeScreen -> Minesweeper (Direct Game)
     * MainHomeScreen -> Sudoku (Sudoku Game with its own menu)
     */

    // Connect MainHomeScreen to SnakeHome
    QObject::connect(mainHome, &MainHomeScreen::navigateToSnake, [&]() {
        stackedWidget.setCurrentIndex(snakeHomeIndex);
    });

    // Connect MainHomeScreen to Minesweeper
    QObject::connect(mainHome, &MainHomeScreen::navigateToMinesweeper, [&]() {
        stackedWidget.setCurrentIndex(minesweeperIndex);
        minesweeperGame->startNewGame();
    });

    // Connect MainHomeScreen to Sudoku
    QObject::connect(mainHome, &MainHomeScreen::navigateToSudoku, [&]() {
        stackedWidget.setCurrentIndex(sudokuIndex);
        // Reset to home screen within Sudoku if needed
        // sudokuGame->resetToHome(); // Uncomment if you have this method
    });

    // Connect Snake game signals
    QObject::connect(snakeHome, &Home::startSnakeGame, [&](int difficulty){
        stackedWidget.setCurrentIndex(snakeGameIndex);
        snakeGame->initGame(difficulty);
    });

    QObject::connect(snakeGame, &Snake::gameOver, [&](int finalScore){
        snakeHome->addHighScore(finalScore);
    });

    QObject::connect(snakeGame, &Snake::backToHome, [&](){
        stackedWidget.setCurrentIndex(snakeHomeIndex);
    });

    QObject::connect(snakeHome, &Home::backToMainHome, [&]() {
        stackedWidget.setCurrentIndex(mainHomeIndex);
    });

    // Connect Minesweeper back button to main home
    QObject::connect(minesweeperGame, &MainWindow::backToMainHome, [&]() {
        stackedWidget.setCurrentIndex(mainHomeIndex);
    });

    // Connect Sudoku back button to main home
    QObject::connect(sudokuGame, &SudokuController::backToMainHome, [&]() {
        stackedWidget.setCurrentIndex(mainHomeIndex);
    });

    // Handle window resizing based on current screen
    QObject::connect(&stackedWidget, &QStackedWidget::currentChanged, [&](int index) {
        QWidget *currentWidget = stackedWidget.widget(index);
        if (currentWidget) {
            stackedWidget.setFixedSize(currentWidget->size());
        }
    });

    // Start with MainHomeScreen
    stackedWidget.setCurrentIndex(mainHomeIndex);
    stackedWidget.show();

    return a.exec();
}
