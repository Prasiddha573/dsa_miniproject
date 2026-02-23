#ifndef SUDOKUCONTROLLER_H
#define SUDOKUCONTROLLER_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
class QStackedWidget;
class HomeScreen;
class GameScreen;
QT_END_NAMESPACE

class SudokuController : public QMainWindow
{
    Q_OBJECT

public:
    explicit SudokuController(QWidget *parent = nullptr);
    ~SudokuController();

signals:
    void backToMainHome();  // Signal to return to main application menu

private slots:
    void onStartGame();
    void onBackToMainMenu();  // New slot for back to main menu
    void onBackToHome();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QStackedWidget *stackedWidget;
    HomeScreen *homeScreen;
    GameScreen *gameScreen;
};

#endif // SUDOKUCONTROLLER_H
