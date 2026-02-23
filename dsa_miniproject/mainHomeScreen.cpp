#include "mainHomeScreen.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QKeyEvent>
#include <QDateTime>
#include <QPainterPath>
#include <QFontDatabase>
#include <QFrame>
#include <QSpacerItem>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>

MainHomeScreen::MainHomeScreen(QWidget *parent)
    : QWidget(parent)
    , m_titleScale(1.0)
    , m_selectedButton(-1)
    , m_topAccent(nullptr)
    , m_exitBtn(nullptr)
{
    // Modern, larger size for better visibility
    setFixedSize(1200, 800);
    setWindowTitle("Game Hub");
    setWindowFlags(Qt::FramelessWindowHint);

    // Initialize UI and Background
    setupUI();
    createBackground();

    // Configure Title Animation (simplified to avoid constant repaints)
    m_titleAnimation = new QPropertyAnimation(this, "titleScale");
    m_titleAnimation->setDuration(2000);
    m_titleAnimation->setKeyValueAt(0, 1.0);
    m_titleAnimation->setKeyValueAt(0.5, 1.03);
    m_titleAnimation->setKeyValueAt(1, 1.0);
    m_titleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_titleAnimation->setLoopCount(-1);
    m_titleAnimation->start();

    // Create button animations group
    m_buttonAnimations = new QParallelAnimationGroup(this);
}

void MainHomeScreen::setupUI()
{
    // Create Main Vertical Layout with modern spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 30, 50, 40);

    // Top decorative accent
    m_topAccent = new QFrame();
    m_topAccent->setFixedHeight(4);
    m_topAccent->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "       stop:0 #4158D0, stop:0.46 #C850C0, stop:1 #FFCC70);"
        "   border-radius: 2px;"
        "}"
        );
    mainLayout->addWidget(m_topAccent);

    // Title
    m_titleLabel = new QLabel("GAME HUB");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 72px;"
        "font-weight: bold;"
        "font-family: 'Arial Black';"
        "background: transparent;"
        "letter-spacing: 4px;"
        "margin-top: 20px;"
        "margin-bottom: 10px;"
        );
    mainLayout->addWidget(m_titleLabel);

    // Subtitle
    m_subtitleLabel = new QLabel("SELECT YOUR GAME");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 180);"
        "font-size: 18px;"
        "letter-spacing: 2px;"
        "font-family: 'Arial';"
        "font-weight: 300;"
        "background: transparent;"
        "margin-bottom: 40px;"
        );
    mainLayout->addWidget(m_subtitleLabel);

    // Game Cards Layout
    QHBoxLayout *gamesLayout = new QHBoxLayout();
    gamesLayout->setSpacing(25);
    gamesLayout->setAlignment(Qt::AlignCenter);

    // Snake Card
    m_snakeBtn = createGameCard(
        "SNAKE",
        "🐍",
        "#2ecc71",
        "#27ae60",
        "Classic Snake Game"
        );
    gamesLayout->addWidget(m_snakeBtn);

    // Minesweeper Card
    m_minesweeperBtn = createGameCard(
        "MINESWEEPER",
        "💣",
        "#e74c3c",
        "#c0392b",
        "Avoid the Mines"
        );
    gamesLayout->addWidget(m_minesweeperBtn);

    // Sudoku Card
    m_sudokuBtn = createGameCard(
        "SUDOKU",
        "🔢",
        "#3498db",
        "#2980b9",
        "Number Puzzle"
        );
    gamesLayout->addWidget(m_sudokuBtn);

    mainLayout->addLayout(gamesLayout);
    mainLayout->addStretch();

    // Bottom Buttons Layout
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignCenter);
    bottomLayout->setSpacing(20);

    // Exit Button
    m_exitBtn = new QPushButton("EXIT");
    m_exitBtn->setCursor(Qt::PointingHandCursor);
    m_exitBtn->setFixedSize(150, 50);
    m_exitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #7f8c8d;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #95a5a6;"
        "}"
        );
    connect(m_exitBtn, &QPushButton::clicked, this, &MainHomeScreen::onExitClicked);

    bottomLayout->addWidget(m_exitBtn);
    mainLayout->addLayout(bottomLayout);

    // Credit Label
    m_creditLabel = new QLabel("Developed By Siddhant Timalsina");
    m_creditLabel->setAlignment(Qt::AlignCenter);
    m_creditLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 100);"
        "font-size: 12px;"
        "font-style: italic;"
        "margin-top: 20px;"
        );
    mainLayout->addWidget(m_creditLabel);

    // Button Group
    m_gameGroup = new QButtonGroup(this);
    m_gameGroup->addButton(m_snakeBtn, 0);
    m_gameGroup->addButton(m_minesweeperBtn, 1);
    m_gameGroup->addButton(m_sudokuBtn, 2);

    connect(m_gameGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &MainHomeScreen::onGameButtonClicked);
}

QPushButton* MainHomeScreen::createGameCard(const QString &title, const QString &emoji,
                                            const QString &color1, const QString &color2,
                                            const QString &subtitle)
{
    QPushButton *card = new QPushButton(this);
    card->setCursor(Qt::PointingHandCursor);
    card->setFixedSize(280, 320);

    // Create layout for card content
    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 30, 20, 20);
    layout->setAlignment(Qt::AlignCenter);

    // Emoji
    QLabel *emojiLabel = new QLabel(emoji);
    emojiLabel->setAlignment(Qt::AlignCenter);
    emojiLabel->setStyleSheet(
        "font-size: 70px;"
        "background: transparent;"
        "color: white;"
        );
    layout->addWidget(emojiLabel);

    // Title
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: white;"
        "background: transparent;"
        );
    layout->addWidget(titleLabel);

    // Subtitle
    QLabel *subtitleLabel = new QLabel(subtitle);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "font-size: 13px;"
        "color: rgba(255, 255, 255, 180);"
        "background: transparent;"
        );
    layout->addWidget(subtitleLabel);

    // Style the card
    QString cardStyle = QString(
                            "QPushButton {"
                            "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %1, stop:1 %2);"
                            "   border-radius: 20px;"
                            "   border: 2px solid rgba(255, 255, 255, 30);"
                            "}"
                            "QPushButton:hover {"
                            "   border: 2px solid rgba(255, 255, 255, 150);"
                            "}"
                            ).arg(color1).arg(color2);
    card->setStyleSheet(cardStyle);

    return card;
}

void MainHomeScreen::createBackground()
{
    // Simple solid background (no gradients to avoid painter conflicts)
    m_backgroundColor = QColor(26, 30, 43);
}

void MainHomeScreen::onGameButtonClicked(int id)
{
    QString message;
    switch(id) {
    case 0:
        message = "🐍 LET'S PLAY SNAKE! 🐍";
        break;
    case 1:
        message = "💣 MINESWEEPER - GOOD LUCK! 💣";
        break;
    case 2:
        message = "🔢 SUDOKU - CHALLENGE YOUR MIND! 🔢";
        break;
    }

    m_subtitleLabel->setText(message);
    m_subtitleLabel->setStyleSheet(
        "color: #f1c40f;"
        "font-size: 18px;"
        "letter-spacing: 2px;"
        "font-family: 'Arial';"
        "font-weight: 500;"
        "background: transparent;"
        );

    QTimer::singleShot(1500, [this]() {
        m_subtitleLabel->setText("SELECT YOUR GAME");
        m_subtitleLabel->setStyleSheet(
            "color: rgba(255, 255, 255, 180);"
            "font-size: 18px;"
            "letter-spacing: 2px;"
            "font-family: 'Arial';"
            "font-weight: 300;"
            "background: transparent;"
            );
    });

    // Navigate
    QTimer::singleShot(200, [this, id]() {
        switch(id) {
        case 0: emit navigateToSnake(); break;
        case 1: emit navigateToMinesweeper(); break;
        case 2: emit navigateToSudoku(); break;
        }
    });
}

void MainHomeScreen::onExitClicked()
{
    QApplication::quit();
}

void MainHomeScreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_backgroundColor);

    // Draw simple decorative elements (no complex gradients or animations)
    painter.setPen(QPen(QColor(255, 255, 255, 20), 1));

    // Draw some subtle diagonal lines
    for (int i = -height(); i < width(); i += 50) {
        painter.drawLine(i, 0, i + height(), height());
    }

    // Draw some simple circles
    painter.setBrush(QColor(52, 152, 219, 10));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(100, 100, 200, 200);
    painter.drawEllipse(width() - 300, height() - 300, 250, 250);
}

void MainHomeScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void MainHomeScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_1 || event->key() == Qt::Key_S) {
        onGameButtonClicked(0);
    } else if (event->key() == Qt::Key_2 || event->key() == Qt::Key_M) {
        onGameButtonClicked(1);
    } else if (event->key() == Qt::Key_3 || event->key() == Qt::Key_U) {
        onGameButtonClicked(2);
    } else if (event->key() == Qt::Key_Escape) {
        onExitClicked();
    }
    QWidget::keyPressEvent(event);
}
