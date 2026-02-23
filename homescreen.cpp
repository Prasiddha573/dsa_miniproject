#include "homescreen.h"
#include "gamesizes.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QDebug>

HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent)
    , m_titleScale(1.0)
{
    setupUI();
    createBackground();

    loadBestTimes();
    updateBestTimeDisplay();

    m_titleAnimation = new QPropertyAnimation(this, "titleScale");
    m_titleAnimation->setDuration(2000);
    m_titleAnimation->setKeyValueAt(0, 1.0);
    m_titleAnimation->setKeyValueAt(0.5, 1.1);
    m_titleAnimation->setKeyValueAt(1, 1.0);
    m_titleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_titleAnimation->setLoopCount(-1);
    m_titleAnimation->start();

    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(600, 500); // Increased minimum width from 400 to 600
}

void HomeScreen::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(GameSizes::standardSpacing() * 1.5); // Increased spacing
    mainLayout->setContentsMargins(GameSizes::largeMargin() * 2,  // Increased horizontal margins
                                   GameSizes::largeMargin(),
                                   GameSizes::largeMargin() * 2,
                                   GameSizes::largeMargin());

    // Title
    m_titleLabel = new QLabel("SUDOKU PUZZLE");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    int titleFontSize = GameSizes::titleFontSize() * 2.5; // Increased font size
    QString titleStyle = QString(
                             "color: white;"
                             "font-size: %1px;"
                             "font-weight: bold;"
                             "font-family: 'Arial Black';"
                             "background: transparent;"
                             "letter-spacing: 4px;"
                             "margin-bottom: 15px;"
                             ).arg(titleFontSize);

    m_titleLabel->setStyleSheet(titleStyle);
    addNeonEffect(m_titleLabel);
    mainLayout->addWidget(m_titleLabel);

    // Subtitle
    m_subtitleLabel = new QLabel("Classic Puzzle Game");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);

    int subtitleFontSize = GameSizes::buttonFontSize() + 6; // Increased font size
    QString subtitleStyle = QString(
                                "color: #3498db;"
                                "font-size: %1px;"
                                "letter-spacing: 2px;"
                                "font-family: 'Arial';"
                                "background: transparent;"
                                "margin-bottom: 25px;"
                                ).arg(subtitleFontSize);

    m_subtitleLabel->setStyleSheet(subtitleStyle);
    mainLayout->addWidget(m_subtitleLabel);

    // Best Time with trophy
    QHBoxLayout *bestTimeLayout = new QHBoxLayout();
    bestTimeLayout->setAlignment(Qt::AlignCenter);
    bestTimeLayout->setSpacing(GameSizes::standardSpacing() * 2);

    QLabel *trophyLabel = new QLabel("🏆");
    int trophyFontSize = GameSizes::titleFontSize() + 10; // Increased font size
    trophyLabel->setStyleSheet(QString("font-size: %1px; background: transparent;").arg(trophyFontSize));
    bestTimeLayout->addWidget(trophyLabel);

    m_bestTimeTitle = new QLabel("BEST TIME");
    int bestTimeTitleFontSize = GameSizes::titleFontSize(); // Increased font size
    m_bestTimeTitle->setStyleSheet(
        QString("color: #f39c12;"
                "font-size: %1px;"
                "font-weight: bold;"
                "background: transparent;").arg(bestTimeTitleFontSize)
        );
    bestTimeLayout->addWidget(m_bestTimeTitle);

    mainLayout->addLayout(bestTimeLayout);

    // Best time value
    m_bestTimeValue = new QLabel("00:00");
    m_bestTimeValue->setAlignment(Qt::AlignCenter);

    int bestTimeValueFontSize = GameSizes::titleFontSize() + 30; // Increased font size
    m_bestTimeValue->setStyleSheet(
        QString("color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffd700, stop:1 #ffa500);"
                "font-size: %1px;"
                "font-weight: 900;"
                "background: transparent;"
                "margin-bottom: 25px;").arg(bestTimeValueFontSize)
        );
    mainLayout->addWidget(m_bestTimeValue);

    // Difficulty selection prompt
    QLabel *difficultyPrompt = new QLabel("SELECT DIFFICULTY");
    difficultyPrompt->setAlignment(Qt::AlignCenter);

    int promptFontSize = GameSizes::buttonFontSize() + 4; // Increased font size
    difficultyPrompt->setStyleSheet(
        QString("color: #3498db;"
                "font-size: %1px;"
                "font-weight: bold;"
                "letter-spacing: 2px;"
                "margin-top: 25px;"
                "margin-bottom: 15px;"
                "background: transparent;").arg(promptFontSize)
        );
    mainLayout->addWidget(difficultyPrompt);

    // Difficulty radio buttons (horizontal)
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->setSpacing(GameSizes::standardSpacing() * 3); // Increased spacing
    difficultyLayout->setAlignment(Qt::AlignCenter);

    m_difficultyGroup = new QButtonGroup(this);

    auto setupRadio = [&](QRadioButton* rb, const QString &color, int id) {
        int radioFontSize = GameSizes::buttonFontSize() + 4; // Increased font size
        rb->setStyleSheet(QString(
                              "QRadioButton { color: %1; font-size: %2px; font-weight: bold; spacing: 12px; background: transparent; }"
                              "QRadioButton::indicator { width: 20px; height: 20px; }"
                              "QRadioButton::indicator:checked { background-color: %1; border-radius: 10px; }"
                              ).arg(color).arg(radioFontSize));
        m_difficultyGroup->addButton(rb, id);
        difficultyLayout->addWidget(rb);
    };

    m_easyRadio = new QRadioButton("EASY");
    m_mediumRadio = new QRadioButton("MEDIUM");
    m_hardRadio = new QRadioButton("HARD");

    setupRadio(m_easyRadio, "#2ecc71", 0);
    setupRadio(m_mediumRadio, "#f39c12", 1);
    setupRadio(m_hardRadio, "#e74c3c", 2);
    m_mediumRadio->setChecked(true);

    mainLayout->addLayout(difficultyLayout);
    connect(m_difficultyGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &HomeScreen::onDifficultyChanged);

    mainLayout->addStretch();

    // Start button
    m_startBtn = new QPushButton("START GAME");
    m_startBtn->setCursor(Qt::PointingHandCursor);
    m_startBtn->setFixedHeight(GameSizes::controlButtonHeight() + 20);
    m_startBtn->setMinimumWidth(GameSizes::controlButtonWidth() + 100); // Increased width

    int startBtnFontSize = GameSizes::buttonFontSize() + 8; // Increased font size
    m_startBtn->setStyleSheet(
        QString("QPushButton {"
                "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2ecc71, stop:1 #27ae60);"
                "   color: white;"
                "   border-radius: %1px;"
                "   font-size: %2px;"
                "   font-weight: bold;"
                "   border: none;"
                "   padding: 12px 30px;"
                "}"
                "QPushButton:hover {"
                "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #27ae60, stop:1 #2ecc71);"
                "}"
                "QPushButton:pressed {"
                "   background-color: #1e8449;"
                "}").arg(GameSizes::controlButtonHeight() / 2).arg(startBtnFontSize)
        );

    // Add shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(3, 3);
    m_startBtn->setGraphicsEffect(shadow);

    mainLayout->addWidget(m_startBtn, 0, Qt::AlignCenter);
    connect(m_startBtn, &QPushButton::clicked, this, &HomeScreen::onStartClicked);

    mainLayout->addSpacing(GameSizes::standardSpacing());

    // Back to Menu button (replacing Exit)
    m_backToMenuBtn = new QPushButton("← BACK TO MENU");
    m_backToMenuBtn->setCursor(Qt::PointingHandCursor);
    m_backToMenuBtn->setFixedHeight(GameSizes::controlButtonHeight() + 15);
    m_backToMenuBtn->setMinimumWidth(GameSizes::controlButtonWidth() + 80); // Increased width

    int backBtnFontSize = GameSizes::buttonFontSize() + 4; // Increased font size
    m_backToMenuBtn->setStyleSheet(
        QString("QPushButton {"
                "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9);"
                "   color: white;"
                "   border-radius: %1px;"
                "   font-size: %2px;"
                "   font-weight: bold;"
                "   border: none;"
                "   padding: 10px 25px;"
                "}"
                "QPushButton:hover {"
                "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2980b9, stop:1 #3498db);"
                "}"
                "QPushButton:pressed {"
                "   background-color: #1f5a7e;"
                "}").arg(GameSizes::controlButtonHeight() / 2).arg(backBtnFontSize)
        );

    // Add shadow effect
    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect();
    shadow2->setBlurRadius(12);
    shadow2->setColor(QColor(0, 0, 0, 70));
    shadow2->setOffset(2, 2);
    m_backToMenuBtn->setGraphicsEffect(shadow2);

    mainLayout->addWidget(m_backToMenuBtn, 0, Qt::AlignCenter);
    connect(m_backToMenuBtn, &QPushButton::clicked, this, &HomeScreen::onBackToMenuClicked);

    // Developer credit
    m_creditLabel = new QLabel("Developed by Prasiddha Timalsina");
    m_creditLabel->setAlignment(Qt::AlignCenter);

    int creditFontSize = qMax(12, GameSizes::buttonFontSize()); // Increased font size
    m_creditLabel->setStyleSheet(
        QString("color: rgba(255, 255, 255, 150);"
                "font-size: %1px;"
                "font-style: italic;"
                "margin-top: 25px;"
                "background: transparent;").arg(creditFontSize)
        );
    mainLayout->addWidget(m_creditLabel);
}

QString HomeScreen::selectedDifficulty() const
{
    int id = m_difficultyGroup->checkedId();
    if (id == 0) return "easy";
    if (id == 1) return "medium";
    return "hard";
}

void HomeScreen::updateBestTime(const QString &difficulty, int seconds)
{
    if (!m_bestTimes.contains(difficulty) || seconds < m_bestTimes[difficulty].seconds) {
        BestTimeEntry entry;
        entry.seconds = seconds;
        entry.date = QDateTime::currentDateTime().toString("dd/MM/yy");
        m_bestTimes[difficulty] = entry;
        saveBestTimes();
        updateBestTimeDisplay();
    }
}

void HomeScreen::loadBestTimes()
{
    QFile file("besttimes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 3) {
                QString diff = parts[0];
                BestTimeEntry entry;
                entry.seconds = parts[1].toInt();
                entry.date = parts[2];
                m_bestTimes[diff] = entry;
            }
        }
        file.close();
    }
}

void HomeScreen::saveBestTimes()
{
    QFile file("besttimes.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = m_bestTimes.begin(); it != m_bestTimes.end(); ++it) {
            out << it.key() << "," << it.value().seconds << "," << it.value().date << "\n";
        }
        file.close();
    }
}

void HomeScreen::updateBestTimeDisplay()
{
    QString diff = selectedDifficulty();
    if (m_bestTimes.contains(diff)) {
        int secs = m_bestTimes[diff].seconds;
        int minutes = secs / 60;
        int seconds = secs % 60;
        m_bestTimeValue->setText(QString("%1:%2")
                                     .arg(minutes, 2, 10, QChar('0'))
                                     .arg(seconds, 2, 10, QChar('0')));
    } else {
        m_bestTimeValue->setText("--:--");
    }
}

void HomeScreen::createBackground()
{
    // Gradient will be updated in resizeEvent
    m_backgroundGradient = QLinearGradient(0, 0, width(), height());
    m_backgroundGradient.setColorAt(0.0, QColor(20, 30, 40));
    m_backgroundGradient.setColorAt(0.3, QColor(30, 40, 60));
    m_backgroundGradient.setColorAt(0.6, QColor(40, 30, 60));
    m_backgroundGradient.setColorAt(1.0, QColor(20, 20, 40));
}

void HomeScreen::addNeonEffect(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(30);
    effect->setColor(QColor(0, 255, 255, 100));
    effect->setOffset(0);
    widget->setGraphicsEffect(effect);
}

void HomeScreen::onDifficultyChanged(int id)
{
    Q_UNUSED(id);
    updateBestTimeDisplay();

    QString diffText = (id == 0) ? "EASY" : (id == 1) ? "MEDIUM" : "HARD";
    m_subtitleLabel->setText(QString("DIFFICULTY: %1").arg(diffText));
    QTimer::singleShot(2000, [this]() {
        m_subtitleLabel->setText("Classic Puzzle Game");
    });
}

void HomeScreen::onStartClicked()
{
    emit startGameClicked();
}

void HomeScreen::onBackToMenuClicked()
{
    emit backToMenuClicked();
}

void HomeScreen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), m_backgroundGradient);

    // Floating particles - scaled with screen size
    painter.setPen(Qt::NoPen);
    int particleCount = qBound(8, width() / 80, 20); // Increased particle count
    int particleSize = qBound(25, width() / 25, 60); // Increased particle size

    for (int i = 0; i < particleCount; ++i) {
        int x = (i * 120 + QDateTime::currentMSecsSinceEpoch() / 15) % width();
        int y = (i * 80) % height();
        painter.setBrush(QColor(52, 152, 219, 20));
        painter.drawEllipse(x, y, particleSize, particleSize);
    }
}

void HomeScreen::resizeEvent(QResizeEvent *event)
{
    m_backgroundGradient.setFinalStop(width(), height());

    // Update title font size on resize
    if (m_titleLabel) {
        int newTitleSize = GameSizes::titleFontSize() * 2.5;
        QString style = m_titleLabel->styleSheet();
        style.replace(QRegExp("font-size: \\d+px"), QString("font-size: %1px").arg(newTitleSize));
        m_titleLabel->setStyleSheet(style);
    }

    QWidget::resizeEvent(event);
}

void HomeScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onStartClicked();
    } else if (event->key() == Qt::Key_Escape) {
        onBackToMenuClicked();
    }
    QWidget::keyPressEvent(event);
}
