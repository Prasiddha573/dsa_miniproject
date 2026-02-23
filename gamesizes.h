#ifndef GAMESIZES_H
#define GAMESIZES_H

#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QDebug>

class GameSizes {
public:
    // Get screen geometry
    static QRect screenGeometry() {
        return QApplication::primaryScreen()->geometry();
    }

    // Calculate relative sizes based on screen dimensions
    static int boardSize() {
        // Board takes 60% of the smaller screen dimension
        QRect screen = screenGeometry();
        int minScreenDim = qMin(screen.width(), screen.height());
        // Add some margin for UI elements
        int maxBoardSize = minScreenDim - 200;
        return qMin(600, maxBoardSize);
    }

    static int cellSize() {
        return boardSize() / 9;  // For Sudoku (9x9 grid)
    }

    static int buttonSize() {
        QRect screen = screenGeometry();
        return qMin(70, static_cast<int>(screen.width() * 0.07));
    }

    static int controlButtonWidth() {
        QRect screen = screenGeometry();
        return qMin(230, static_cast<int>(screen.width() * 0.2));
    }

    static int controlButtonHeight() {
        QRect screen = screenGeometry();
        return qMin(45, static_cast<int>(screen.height() * 0.05));
    }

    static int timerWidth() {
        QRect screen = screenGeometry();
        return qMin(120, static_cast<int>(screen.width() * 0.15));
    }

    // Snake specific sizes
    static int snakeCellSize() {
        QRect screen = screenGeometry();
        int baseSize = qMin(25, static_cast<int>(screen.width() * 0.025));
        // Ensure minimum size for visibility
        return qMax(15, baseSize);
    }

    // Minesweeper specific sizes
    static int mineCellSize() {
        QRect screen = screenGeometry();
        int baseSize = qMin(35, static_cast<int>(screen.width() * 0.035));
        return qMax(20, baseSize);
    }

    // Font sizes
    static int titleFontSize() {
        QRect screen = screenGeometry();
        return qMin(24, static_cast<int>(screen.width() * 0.02));
    }

    static int buttonFontSize() {
        QRect screen = screenGeometry();
        return qMin(14, static_cast<int>(screen.width() * 0.012));
    }

    static int numberFontSize() {
        QRect screen = screenGeometry();
        return qMin(24, static_cast<int>(screen.width() * 0.02));
    }

    // Game window sizes
    static int gameWindowWidth() {
        QRect screen = screenGeometry();
        return qMin(800, static_cast<int>(screen.width() * 0.8));
    }

    static int gameWindowHeight() {
        QRect screen = screenGeometry();
        return qMin(600, static_cast<int>(screen.height() * 0.8));
    }

    // Margin sizes
    static int standardMargin() {
        QRect screen = screenGeometry();
        return qMin(20, static_cast<int>(screen.width() * 0.02));
    }

    static int largeMargin() {
        QRect screen = screenGeometry();
        return qMin(40, static_cast<int>(screen.width() * 0.04));
    }

    // Spacing between elements
    static int standardSpacing() {
        QRect screen = screenGeometry();
        return qMin(10, static_cast<int>(screen.width() * 0.01));
    }

    // Helper to get scaled size while maintaining aspect ratio
    static QSize scaledSize(int baseWidth, int baseHeight) {
        QRect screen = screenGeometry();
        float widthScale = static_cast<float>(screen.width()) / 1920.0f; // Assuming 1920x1080 as base
        float heightScale = static_cast<float>(screen.height()) / 1080.0f;
        float scale = qMin(widthScale, heightScale);

        int newWidth = qMax(static_cast<int>(baseWidth * scale), baseWidth / 2);
        int newHeight = qMax(static_cast<int>(baseHeight * scale), baseHeight / 2);

        return QSize(newWidth, newHeight);
    }

    // Get optimal grid size based on screen size
    static int optimalGridSize(int minCells, int maxCells) {
        QRect screen = screenGeometry();
        int minDim = qMin(screen.width(), screen.height());

        // More cells on larger screens
        if (minDim < 600) {
            return minCells;  // Small screen
        } else if (minDim < 900) {
            return (minCells + maxCells) / 2;  // Medium screen
        } else {
            return maxCells;  // Large screen
        }
    }

    // Debug function to print current sizes
    static void debugSizes() {
        qDebug() << "=== GameSizes Debug ===";
        qDebug() << "Screen:" << screenGeometry();
        qDebug() << "Board Size:" << boardSize();
        qDebug() << "Cell Size:" << cellSize();
        qDebug() << "Button Size:" << buttonSize();
        qDebug() << "Control Button:" << controlButtonWidth() << "x" << controlButtonHeight();
        qDebug() << "Timer Width:" << timerWidth();
        qDebug() << "Snake Cell:" << snakeCellSize();
        qDebug() << "Mine Cell:" << mineCellSize();
        qDebug() << "Font Sizes - Title:" << titleFontSize() << "Button:" << buttonFontSize() << "Number:" << numberFontSize();
    }
};

#endif // GAMESIZES_H
