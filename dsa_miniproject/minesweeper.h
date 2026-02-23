#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVector>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QFont>
#include <QFrame>
#include <QProgressBar>
#include <QSpacerItem>
#include <QScrollArea>
#include <QEasingCurve>
#include <QGraphicsDropShadowEffect>

/**
 * @enum Difficulty
 * @brief Represents the game difficulty levels
 */
enum Difficulty {
    EASY,      ///< Easy difficulty - 6x6 grid with 6 mines
    MEDIUM,    ///< Medium difficulty - 10x10 grid with 15 mines
    HARD       ///< Hard difficulty - 14x14 grid with 30 mines
};

/**
 * @struct Move
 * @brief Stores a single move for undo functionality
 */
struct Move {
    int r;  ///< Row coordinate
    int c;  ///< Column coordinate
};

/**
 * @namespace ThemeColors
 * @brief Contains color constants for the dark theme
 */
namespace ThemeColors {
const QString DARK_BG = "#1e1e2e";          ///< Dark background color
const QString DARKER_BG = "#181825";        ///< Darker background for contrast
const QString SURFACE = "#313244";          ///< Surface color for cards
const QString PRIMARY = "#89b4fa";          ///< Primary blue accent
const QString SECONDARY = "#94e2d5";        ///< Secondary teal accent
const QString ACCENT = "#cba6f7";           ///< Purple accent
const QString ERROR = "#f38ba8";             ///< Error red color
const QString SUCCESS = "#a6e3a1";           ///< Success green color
const QString WARNING = "#fab387";           ///< Warning orange color
const QString TEXT = "#cdd6f4";              ///< Primary text color
const QString TEXT_SECONDARY = "#7f849c";    ///< Secondary text color
const QString BORDER = "#45475a";            ///< Border color
const QString HOVER = "#45475a";             ///< Hover state color
}

/**
 * @class MainWindow
 * @brief Main window class for the Minesweeper game
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent widget (default nullptr)
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~MainWindow();

    /**
     * @brief Public method to start/reset the game
     * Can be called from main.cpp when navigating to Minesweeper
     */
    void startNewGame();

    /**
     * @brief Get the current difficulty level
     * @return Current difficulty
     */
    Difficulty getCurrentDifficulty() const { return difficulty; }

    /**
     * @brief Set difficulty and start new game
     * @param diff Difficulty level to set
     */
    void setDifficultyAndStart(Difficulty diff);

signals:
    /**
     * @brief Signal emitted when back button is clicked
     * Connected to main.cpp to return to main home screen
     */
    void backToMainHome();

private slots:
    /**
     * @brief Handles back button click
     */
    void onBackToMainClicked();

    /**
     * @brief Updates the game timer display
     */
    void updateTimer();

    /**
     * @brief Handles left-click on a cell
     */
    void handleCellClick();

    /**
     * @brief Handles right-click (flagging) on a cell
     * @param pos Mouse position (unused)
     */
    void handleCellRightClick(const QPoint &pos);

    /**
     * @brief Sets game to Easy difficulty
     */
    void setEasy();

    /**
     * @brief Sets game to Medium difficulty
     */
    void setMedium();

    /**
     * @brief Sets game to Hard difficulty
     */
    void setHard();

    /**
     * @brief Provides a hint by highlighting a safe cell
     */
    void giveHint();

    /**
     * @brief Undoes the last move
     */
    void undoLastMove();

    /**
     * @brief Animates button press
     * @param btn Button to animate
     */
    void animateButton(QPushButton* btn);

    /**
     * @brief Displays game statistics
     */
    void showStatistics();

private:
    // ==================== UI Components ====================
    QWidget *centralWidget;              ///< Central widget
    QVBoxLayout *mainLayout;             ///< Main vertical layout
    QHBoxLayout *topLayout;              ///< Top layout for stats
    QGridLayout *gridLayout;              ///< Grid layout for game board
    QHBoxLayout *bottomLayout;            ///< Bottom layout for action buttons
    QHBoxLayout *statsLayout;             ///< Statistics layout

    // Labels
    QLabel *titleLabel;                   ///< Game title label

    // Stat Buttons (styled like difficulty buttons)
    QPushButton *m_mineStatBtn;           ///< Mine counter button
    QPushButton *m_timerStatBtn;          ///< Timer button
    QPushButton *m_bestStatBtn;           ///< Best score button

    // Control Buttons
    QPushButton *restartButton;            ///< Restart game button
    QPushButton *easyBtn;                  ///< Easy difficulty button
    QPushButton *mediumBtn;                 ///< Medium difficulty button
    QPushButton *hardBtn;                   ///< Hard difficulty button
    QPushButton *hintBtn;                   ///< Hint button
    QPushButton *undoBtn;                   ///< Undo button
    QPushButton *statsBtn;                  ///< Statistics button
    QPushButton *backButton;                ///< Back to main menu button

    // Game Board Data
    QVector<QVector<QPushButton*>> buttons;  ///< 2D array of cell buttons
    QVector<QVector<int>> board;              ///< Game board state (-1 for mine, 0-8 for adjacent mines)
    QVector<QVector<bool>> revealed;          ///< Whether each cell is revealed
    QVector<QVector<bool>> flagged;           ///< Whether each cell is flagged

    // Game State
    QTimer *timer;                            ///< Game timer
    int secondsElapsed;                        ///< Seconds elapsed in current game
    int rows;                                  ///< Number of rows in current grid
    int cols;                                  ///< Number of columns in current grid
    int mineCount;                             ///< Number of mines in current game
    int flagsPlaced;                           ///< Number of flags placed
    Difficulty difficulty;                      ///< Current difficulty level

    // Statistics
    int bestTime;                              ///< Best time record
    int gamesPlayed;                           ///< Total games played
    int gamesWon;                              ///< Total games won
    QString scoreFile = "best_score.txt";      ///< File for best score storage
    QString statsFile = "stats.txt";           ///< File for statistics storage

    // Move History
    QVector<Move> moveHistory;                 ///< History of moves for undo

    // ==================== Private Methods ====================
    /**
     * @brief Initializes a new game
     */
    void initializeGame();

    /**
     * @brief Sets up game parameters based on difficulty
     * @param diff Difficulty level
     */
    void setupDifficulty(Difficulty diff);

    /**
     * @brief Randomly places mines on the board
     */
    void placeMines();

    /**
     * @brief Calculates adjacent mine counts for each cell
     */
    void calculateNumbers();

    /**
     * @brief Reveals a cell and handles cascading reveals
     * @param r Row coordinate
     * @param c Column coordinate
     */
    void revealCell(int r, int c);

    /**
     * @brief Checks if the player has won
     */
    void checkWin();

    /**
     * @brief Handles game over
     * @param win True if player won, false if they hit a mine
     */
    void gameOver(bool win = false);

    /**
     * @brief Styles a regular button
     * @param btn Button to style
     */
    void styleButton(QPushButton* btn);

    /**
     * @brief Styles an action button with specific color
     * @param btn Button to style
     * @param color Color to use
     */
    void styleActionButton(QPushButton* btn, const QString& color = ThemeColors::PRIMARY);

    /**
     * @brief Loads best score from file
     */
    void loadBestScore();

    /**
     * @brief Saves best score to file
     * @param time Time to save
     */
    void saveBestScore(int time);

    /**
     * @brief Finds a safe cell for hint
     * @param r Reference to store row
     * @param c Reference to store column
     * @return True if safe cell found
     */
    bool safeCell(int &r, int &c);

    /**
     * @brief Updates mine counter display
     */
    void updateMineDisplay();

    /**
     * @brief Loads statistics from file
     */
    void loadStatistics();

    /**
     * @brief Saves statistics to file
     * @param won True if game was won
     */
    void saveStatistics(bool won);

    /**
     * @brief Applies global style sheet
     */
    void applyGlobalStyle();

    /**
     * @brief Creates a separator line
     * @return Separator frame
     */
    QFrame* createSeparator();
};

#endif // MINESWEEPER_H
