#ifndef MAINHOMESCREEN_H
#define MAINHOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QTimer>

class MainHomeScreen : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float titleScale READ titleScale WRITE setTitleScale)

public:
    explicit MainHomeScreen(QWidget *parent = nullptr);

    float titleScale() const { return m_titleScale; }
    void setTitleScale(float scale) { m_titleScale = scale; update(); }

signals:
    void navigateToSnake();
    void navigateToMinesweeper();
    void navigateToSudoku();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onGameButtonClicked(int id);
    void onExitClicked();

private:
    void setupUI();
    void createBackground();
    QPushButton* createGameCard(const QString &title, const QString &emoji,
                                const QString &color1, const QString &color2,
                                const QString &subtitle);

    // UI Elements
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QPushButton *m_snakeBtn;
    QPushButton *m_minesweeperBtn;
    QPushButton *m_sudokuBtn;
    QPushButton *m_exitBtn;
    QLabel *m_creditLabel;
    QFrame *m_topAccent;

    // Button Group
    QButtonGroup *m_gameGroup;

    // Animations
    QPropertyAnimation *m_titleAnimation;
    QParallelAnimationGroup *m_buttonAnimations;
    float m_titleScale;
    int m_selectedButton;

    // Background
    QColor m_backgroundColor;
};

#endif // MAINHOMESCREEN_H
