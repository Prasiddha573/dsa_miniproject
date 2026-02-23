#include "snake.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QFontDatabase>
#include <QParallelAnimationGroup>
#include <QApplication>
#include <QDebug>
#include <algorithm>
#include "gamesizes.h"

/**
 * @brief Constructor For The Snake Game Widget
 * @param parent Parent Widget Pointer
 */
Snake::Snake(QWidget *parent)
    : QWidget(parent)
    , m_gameOver(false)
    , m_paused(false)
    , m_score(0)
    , m_gridSize(20)
    , m_cellSize(GameSizes::snakeCellSize())
    , m_headScale(1.0)
    , m_shadowOffset(0)
    , m_currentDifficulty(1)
{
    // Calculate optimal size based on screen
    QSize optimalSize = calculateOptimalSize();
    setFixedSize(optimalSize);

    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet("background-color: transparent;");

    // Initialize Visual Components
    createGradientBackground();

    // Setup Game Timer (don't start yet)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Snake::gameLoop);

    // Initialize Head Animation
    m_headAnimation = new QPropertyAnimation(this, "headScale");
    m_headAnimation->setDuration(300);
    m_headAnimation->setKeyValueAt(0, 1.0);
    m_headAnimation->setKeyValueAt(0.5, 1.2);
    m_headAnimation->setKeyValueAt(1, 1.0);
    m_headAnimation->setEasingCurve(QEasingCurve::OutBounce);
    connect(m_headAnimation, &QPropertyAnimation::finished, this, &Snake::animateHead);

    // Initialize Shadow Animation
    m_shadowAnimation = new QPropertyAnimation(this, "shadowOffset");
    m_shadowAnimation->setDuration(200);
    m_shadowAnimation->setStartValue(0);
    m_shadowAnimation->setEndValue(GameSizes::snakeCellSize() / 6);
    m_shadowAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // Setup Particle Effects Timer
    m_particleTimer = new QTimer(this);
    m_particleTimer->setInterval(50);
    connect(m_particleTimer, &QTimer::timeout, [this]() {
        // Update Particle Positions
        for (auto &particle : m_particles) {
            particle.ry() += 2;
        }

        // Remove Particles That Have Fallen Off Screen
        m_particles.erase(
            std::remove_if(m_particles.begin(), m_particles.end(),
                           [this](const QPointF &p) { return p.y() > height(); }),
            m_particles.end());
        update();
    });

    // Initialize Snake Color Palette
    for (int i = 0; i < 10; ++i) {
        int hue = (i * 36) % 360;
        m_snakeColors[i] = QColor::fromHsv(hue, 255, 200);
    }

    // Initialize with default difficulty (but don't start game yet)
    // We'll initialize when the widget is shown
}

/**
 * @brief Destructor Ensures Proper Cleanup Of Resources
 */
Snake::~Snake()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_particleTimer && m_particleTimer->isActive()) {
        m_particleTimer->stop();
    }
}

/**
 * @brief Show event - called when widget is shown
 */
void Snake::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Start the game when the widget is shown
    initGame(m_currentDifficulty);
}

/**
 * @brief Calculate optimal widget size based on screen
 */
QSize Snake::calculateOptimalSize() const
{
    QRect screenGeo = GameSizes::screenGeometry();
    int boardSize = GameSizes::boardSize();

    // Ensure board fits within screen with margins
    int maxDim = qMin(screenGeo.width() - 100, screenGeo.height() - 100);
    int size = qMin(boardSize, maxDim);

    return QSize(size, size);
}

/**
 * @brief Update grid size based on difficulty
 */
void Snake::updateGridSizeForDifficulty()
{
    int baseGridSize;

    // Adjust grid size based on screen size and difficulty
    QRect screenGeo = GameSizes::screenGeometry();
    int minDim = qMin(screenGeo.width(), screenGeo.height());

    // Base grid size on screen dimensions
    if (minDim < 600) {
        baseGridSize = 15;  // Small screen
    } else if (minDim < 900) {
        baseGridSize = 20;  // Medium screen
    } else {
        baseGridSize = 25;  // Large screen
    }

    // Adjust based on difficulty
    switch(m_currentDifficulty) {
    case 0: // Easy - smaller grid
        m_gridSize = baseGridSize - 3;
        break;
    case 1: // Medium
        m_gridSize = baseGridSize;
        break;
    case 2: // Hard - larger grid
        m_gridSize = baseGridSize + 3;
        break;
    default:
        m_gridSize = baseGridSize;
    }

    // Ensure grid size is reasonable
    m_gridSize = qBound(10, m_gridSize, 30);
}

/**
 * @brief Calculate cell size based on widget size
 */
void Snake::calculateCellSize()
{
    if (m_gridSize <= 0) return;

    m_cellSize = qMin(width(), height()) / m_gridSize;

    // Ensure minimum cell size
    int minCellSize = GameSizes::snakeCellSize();
    if (m_cellSize < minCellSize) {
        m_cellSize = minCellSize;
        // Recalculate grid size if necessary
        m_gridSize = qMin(width(), height()) / m_cellSize;
    }
}

/**
 * @brief Creates The Gradient Background For The Game Area
 */
void Snake::createGradientBackground()
{
    m_backgroundGradient = QLinearGradient(0, 0, width(), height());

    // Color scheme based on difficulty
    switch(m_currentDifficulty) {
    case 0: // Easy - greens
        m_backgroundGradient.setColorAt(0.0, QColor(20, 40, 30));
        m_backgroundGradient.setColorAt(0.5, QColor(30, 50, 40));
        m_backgroundGradient.setColorAt(1.0, QColor(20, 40, 30));
        break;
    case 1: // Medium - blues
        m_backgroundGradient.setColorAt(0.0, QColor(20, 30, 40));
        m_backgroundGradient.setColorAt(0.5, QColor(30, 40, 50));
        m_backgroundGradient.setColorAt(1.0, QColor(20, 30, 40));
        break;
    case 2: // Hard - reds/purples
        m_backgroundGradient.setColorAt(0.0, QColor(40, 20, 30));
        m_backgroundGradient.setColorAt(0.5, QColor(50, 30, 40));
        m_backgroundGradient.setColorAt(1.0, QColor(40, 20, 30));
        break;
    }
}

/**
 * @brief Initializes A New Game With Specified Difficulty
 * @param difficulty Difficulty Level (0 = Easy, 1 = Medium, 2 = Hard)
 */
void Snake::initGame(int difficulty)
{
    // Stop any existing timers
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_particleTimer && m_particleTimer->isActive()) {
        m_particleTimer->stop();
    }

    m_currentDifficulty = difficulty;

    // Update grid size based on difficulty
    updateGridSizeForDifficulty();

    // Calculate cell size
    calculateCellSize();

    // Set Game Speed Based On Difficulty Level
    int speed;
    switch(difficulty) {
    case 0:
        speed = 200;
        break;
    case 1:
        speed = 150;
        break;
    case 2:
        speed = 100;
        break;
    default:
        speed = 150;
    }

    // Initialize Snake Starting Position (centered based on grid size)
    int centerX = m_gridSize / 2;
    int centerY = m_gridSize / 2;

    m_snake.clear();
    m_snake.append(QPoint(centerX, centerY));
    m_snake.append(QPoint(centerX - 1, centerY));
    m_snake.append(QPoint(centerX - 2, centerY));

    // Reset Game State
    m_direction = QPoint(1, 0);
    m_nextDirection = QPoint(1, 0);
    m_score = 0;
    m_gameOver = false;
    m_paused = false;
    m_particles.clear();

    // Update gradient for new difficulty
    createGradientBackground();

    // Spawn first food
    spawnFood();

    // Start Game Components
    updateScore(0);
    m_timer->start(speed);
    m_headAnimation->start();

    emit gameStarted();
    update();
}

/**
 * @brief Pauses The Current Game
 */
void Snake::pauseGame()
{
    if (!m_paused && !m_gameOver) {
        if (m_timer) m_timer->stop();
        if (m_particleTimer) m_particleTimer->stop();
        m_paused = true;
        emit gamePaused();
        update();
    }
}

/**
 * @brief Resumes A Paused Game
 */
void Snake::resumeGame()
{
    if (m_paused && !m_gameOver) {
        if (m_timer) m_timer->start();
        if (m_particleTimer) m_particleTimer->start();
        m_paused = false;
        emit gameResumed();
        update();
    }
}

/**
 * @brief Changes Game Difficulty During Runtime
 * @param difficulty New Difficulty Level
 */
void Snake::setDifficulty(int difficulty)
{
    m_currentDifficulty = difficulty;

    // Update grid size
    updateGridSizeForDifficulty();

    // Update speed
    int speed;
    switch(difficulty) {
    case 0: speed = 200; break;
    case 1: speed = 150; break;
    case 2: speed = 100; break;
    default: speed = 150;
    }

    if (!m_gameOver && !m_paused && m_timer) {
        m_timer->setInterval(speed);
    }

    // Update gradient
    createGradientBackground();
    update();
}

/**
 * @brief Spawns Food At A Random Valid Position On The Grid
 */
void Snake::spawnFood()
{
    if (m_gridSize <= 0) return;

    int maxX = m_gridSize - 1;
    int maxY = m_gridSize - 1;

    if (maxX <= 0 || maxY <= 0) return;

    // Check if snake fills the entire grid (win condition)
    if (m_snake.size() >= m_gridSize * m_gridSize) {
        // Player wins!
        m_timer->stop();
        m_particleTimer->stop();
        m_gameOver = true;
        showWinDialog();
        return;
    }

    // Find Valid Position Not Occupied By Snake
    int attempts = 0;
    bool validPosition = false;

    while (!validPosition && attempts < 1000) {
        int x = QRandomGenerator::global()->bounded(0, maxX + 1);
        int y = QRandomGenerator::global()->bounded(0, maxY + 1);
        m_food = QPoint(x, y);

        if (!m_snake.contains(m_food)) {
            validPosition = true;
        }
        attempts++;
    }

    // If no valid position found after many attempts, game is won
    if (!validPosition) {
        m_timer->stop();
        m_particleTimer->stop();
        m_gameOver = true;
        showWinDialog();
        return;
    }

    // Create Particle Effect For New Food
    int particleCount = qBound(5, m_cellSize / 5, 12);
    for (int i = 0; i < particleCount; ++i) {
        m_particles.append(QPointF(
            m_food.x() * m_cellSize + m_cellSize/2,
            m_food.y() * m_cellSize + m_cellSize/2
            ));
    }
    if (!m_particleTimer->isActive()) {
        m_particleTimer->start();
    }
}

/**
 * @brief Moves The Snake In The Current Direction
 */
void Snake::moveSnake()
{
    if (m_gameOver || m_paused || m_snake.isEmpty()) return;

    // Update Direction If Valid (Prevent Reversing)
    if (m_nextDirection != -m_direction && m_nextDirection != QPoint(0,0)) {
        m_direction = m_nextDirection;
    }

    // Add New Head Position
    QPoint newHead = m_snake.first() + m_direction;
    m_snake.prepend(newHead);

    // Handle Food Collision
    if (newHead == m_food) {
        updateScore(m_score + 1);
        spawnFood();
        m_headAnimation->start();
        m_shadowAnimation->start();
    } else {
        m_snake.removeLast();
    }

    // Check For Collision After Movement
    if (checkCollision()) {
        m_timer->stop();
        m_particleTimer->stop();
        m_gameOver = true;
        showGameOverDialog();
        emit gameOver(m_score);
        update();
    }
}

/**
 * @brief Checks For Wall And Self Collision
 * @return true If Collision Detected, false Otherwise
 */
bool Snake::checkCollision()
{
    if (m_snake.isEmpty()) return true;

    QPoint head = m_snake.first();

    // Check Wall Collision
    if (head.x() < 0 || head.x() >= m_gridSize ||
        head.y() < 0 || head.y() >= m_gridSize) {
        return true;
    }

    // Check Self Collision (skip head at index 0)
    for (int i = 1; i < m_snake.size(); ++i) {
        if (m_snake[i] == head) return true;
    }

    return false;
}

/**
 * @brief Main Game Loop Called By Timer
 */
void Snake::gameLoop()
{
    moveSnake();
    update();
}

/**
 * @brief Resets Head Scale After Animation Completes
 */
void Snake::animateHead()
{
    m_headScale = 1.0;
}

/**
 * @brief Handles Keyboard Input For Game Control
 * @param event Key Press Event
 */
void Snake::keyPressEvent(QKeyEvent *event)
{
    if (!event) return;

    // Handle game over state - restart with R key
    if (m_gameOver) {
        if (event->key() == Qt::Key_R) {
            initGame(m_currentDifficulty);
        }
        return;
    }

    // Set Next Direction Based On Key Press
    if (event->key() == Qt::Key_Up && m_direction != QPoint(0, 1)) {
        m_nextDirection = QPoint(0, -1);
    }
    else if (event->key() == Qt::Key_Down && m_direction != QPoint(0, -1)) {
        m_nextDirection = QPoint(0, 1);
    }
    else if (event->key() == Qt::Key_Left && m_direction != QPoint(1, 0)) {
        m_nextDirection = QPoint(-1, 0);
    }
    else if (event->key() == Qt::Key_Right && m_direction != QPoint(-1, 0)) {
        m_nextDirection = QPoint(1, 0);
    }
    else if (event->key() == Qt::Key_Space) {
        if (m_paused) resumeGame();
        else pauseGame();
    }
    else if (event->key() == Qt::Key_Escape) {
        m_timer->stop();
        m_particleTimer->stop();
        hide();
        emit backToHome();
    }

    QWidget::keyPressEvent(event);
}

/**
 * @brief Handles Widget Resize Events
 * @param event Resize Event
 */
void Snake::resizeEvent(QResizeEvent *event)
{
    // Recalculate cell size when widget is resized
    if (m_gridSize > 0) {
        calculateCellSize();
    }
    createGradientBackground();
    QWidget::resizeEvent(event);
}

/**
 * @brief Renders The Game Graphics
 * @param event Paint Event (Unused)
 */
void Snake::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw Background
    painter.fillRect(rect(), m_backgroundGradient);

    // Draw Grid Overlay (more subtle on larger screens)
    if (m_gridSize > 0 && m_cellSize > 0) {
        int gridAlpha = qBound(10, 30 - (m_cellSize / 5), 30);
        painter.setPen(QPen(QColor(255, 255, 255, gridAlpha), 1));

        int gridWidth = m_gridSize * m_cellSize;
        for (int x = 0; x <= gridWidth; x += m_cellSize) {
            painter.drawLine(x, 0, x, gridWidth);
        }
        for (int y = 0; y <= gridWidth; y += m_cellSize) {
            painter.drawLine(0, y, gridWidth, y);
        }
    }

    int blockSize = m_cellSize - 2;
    int cornerRadius = qBound(4, m_cellSize / 4, 12);

    // Draw Food
    if (m_food.x() >= 0 && m_food.y() >= 0 && m_gridSize > 0) {
        QRadialGradient foodGradient(
            m_food.x() * m_cellSize + m_cellSize/2,
            m_food.y() * m_cellSize + m_cellSize/2,
            m_cellSize * 0.8
            );
        foodGradient.setColorAt(0, Qt::yellow);
        foodGradient.setColorAt(0.7, QColor(255, 200, 0));
        foodGradient.setColorAt(1, QColor(255, 100, 0));

        painter.setBrush(foodGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(
            m_food.x() * m_cellSize + 2,
            m_food.y() * m_cellSize + 2,
            blockSize - 4,
            blockSize - 4
            );
    }

    // Draw Particle Effects
    int particleSize = qBound(2, m_cellSize / 8, 4);
    painter.setBrush(QColor(255, 255, 255, 100));
    for (const auto &particle : m_particles) {
        painter.drawEllipse(particle, particleSize, particleSize);
    }

    // Draw Snake Body
    for (int i = 0; i < m_snake.size(); ++i) {
        QPoint part = m_snake[i];
        QRectF rect(
            part.x() * m_cellSize + 2,
            part.y() * m_cellSize + 2,
            blockSize - 4,
            blockSize - 4
            );

        if (i == 0) {
            // Draw Snake Head With Special Effects
            QRadialGradient headGradient(rect.center(), rect.width());
            headGradient.setColorAt(0, Qt::red);
            headGradient.setColorAt(0.7, QColor(200, 0, 0));
            headGradient.setColorAt(1, QColor(150, 0, 0));

            painter.setBrush(headGradient);

            // Apply Head Scale Animation
            if (m_headScale != 1.0) {
                rect = QRectF(
                    rect.x() - (rect.width() * (m_headScale - 1)) / 2,
                    rect.y() - (rect.height() * (m_headScale - 1)) / 2,
                    rect.width() * m_headScale,
                    rect.height() * m_headScale
                    );
            }

            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(rect, cornerRadius, cornerRadius);

            // Draw Eyes Based On Direction (scaled with cell size)
            painter.setBrush(Qt::white);
            float eyeSize = rect.width() * 0.2;
            float pupilSize = eyeSize * 0.5;

            if (m_direction.x() > 0) {
                // Right Direction
                painter.drawEllipse(rect.right() - eyeSize * 2.5, rect.top() + eyeSize, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2.5, rect.bottom() - eyeSize * 2, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.right() - eyeSize * 2.5 + pupilSize/2, rect.top() + eyeSize + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2.5 + pupilSize/2, rect.bottom() - eyeSize * 2 + pupilSize/2, pupilSize, pupilSize);
            }
            else if (m_direction.x() < 0) {
                // Left Direction
                painter.drawEllipse(rect.left() + eyeSize * 1.5, rect.top() + eyeSize, eyeSize, eyeSize);
                painter.drawEllipse(rect.left() + eyeSize * 1.5, rect.bottom() - eyeSize * 2, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize * 1.5 + pupilSize/2, rect.top() + eyeSize + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.left() + eyeSize * 1.5 + pupilSize/2, rect.bottom() - eyeSize * 2 + pupilSize/2, pupilSize, pupilSize);
            }
            else if (m_direction.y() > 0) {
                // Down Direction
                painter.drawEllipse(rect.left() + eyeSize, rect.bottom() - eyeSize * 2.5, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2, rect.bottom() - eyeSize * 2.5, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize + pupilSize/2, rect.bottom() - eyeSize * 2.5 + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2 + pupilSize/2, rect.bottom() - eyeSize * 2.5 + pupilSize/2, pupilSize, pupilSize);
            }
            else {
                // Up Direction
                painter.drawEllipse(rect.left() + eyeSize, rect.top() + eyeSize * 1.5, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2, rect.top() + eyeSize * 1.5, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize + pupilSize/2, rect.top() + eyeSize * 1.5 + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2 + pupilSize/2, rect.top() + eyeSize * 1.5 + pupilSize/2, pupilSize, pupilSize);
            }

        } else {
            // Draw Snake Body Segments With Gradient Colors
            int colorIndex = i % 10;
            QColor bodyColor = m_snakeColors.contains(colorIndex) ?
                                   m_snakeColors[colorIndex] :
                                   QColor(0, 100, 200);

            QLinearGradient bodyGradient(rect.topLeft(), rect.bottomRight());
            bodyGradient.setColorAt(0, bodyColor.lighter(120));
            bodyGradient.setColorAt(1, bodyColor.darker(120));

            painter.setBrush(bodyGradient);
            painter.setPen(QPen(QColor(255, 255, 255, 50), 1));
            painter.drawRoundedRect(rect, cornerRadius - 2, cornerRadius - 2);
        }
    }

    // Draw Shadow Effect On Food
    if (m_shadowOffset > 0 && m_food.x() >= 0 && m_food.y() >= 0) {
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_food.x() * m_cellSize + m_cellSize/2 + m_shadowOffset,
                            m_food.y() * m_cellSize + m_cellSize/2 + m_shadowOffset,
                            m_cellSize/3, m_cellSize/3);
    }

    // Draw Current Score (scaled with widget)
    painter.setPen(Qt::white);
    int scoreFontSize = qBound(16, width() / 15, 32);
    QFont scoreFont = QFont("Arial", scoreFontSize, QFont::Bold);
    scoreFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    painter.setFont(scoreFont);

    QRect scoreRect(0, height() - scoreFontSize * 2, width(), scoreFontSize * 1.5);
    painter.drawText(scoreRect, Qt::AlignCenter, QString("%1").arg(m_score));

    // Draw Pause Indicator
    if (m_paused) {
        painter.setPen(QPen(QColor(255, 255, 255, 200), 3));
        int pauseFontSize = qBound(24, width() / 10, 48);
        painter.setFont(QFont("Arial", pauseFontSize, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    }
}

/**
 * @brief Updates The Game Score
 * @param newScore New Score Value
 */
void Snake::updateScore(int newScore)
{
    if (newScore != m_score) {
        m_score = newScore;
    }
}

/**
 * @brief Displays The Win Dialog
 */
void Snake::showWinDialog()
{
    // Calculate overlay size based on widget size
    int overlayWidth = qMin(400, width() - 100);
    int overlayHeight = qMin(480, height() - 100);

    // Create Overlay Widget
    QWidget *overlay = new QWidget(this);
    overlay->setFixedSize(overlayWidth, overlayHeight);
    overlay->move((width() - overlayWidth) / 2, (height() - overlayHeight) / 2);

    overlay->setStyleSheet(
        "background-color: #1a1e2b;"
        "border-radius: 40px;"
        "border: 1px solid #2a2f3c;"
        );

    // Add Shadow Effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(40);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 8);
    overlay->setGraphicsEffect(shadowEffect);

    // Setup Main Layout
    QVBoxLayout *layout = new QVBoxLayout(overlay);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 35, 40, 35);

    // Create Win Title
    QLabel *winLabel = new QLabel("🎉 YOU WIN! 🎉");
    winLabel->setFixedHeight(overlayHeight / 8);
    winLabel->setAlignment(Qt::AlignCenter);

    int titleFontSize = qBound(20, overlayWidth / 12, 28);
    winLabel->setStyleSheet(
        QString("QLabel {"
                "   background-color: #2a2f3c;"
                "   color: #2ecc71;"
                "   border-radius: %1px;"
                "   font-size: %2px;"
                "   font-weight: 900;"
                "   font-family: 'Arial Black';"
                "   letter-spacing: 2px;"
                "}").arg(overlayHeight / 16).arg(titleFontSize)
        );
    layout->addWidget(winLabel);

    // Add Decorative Line
    QFrame *line = new QFrame();
    line->setFixedHeight(4);
    line->setFixedWidth(overlayWidth / 2);
    line->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 transparent, stop:0.5 #2ecc71, stop:1 transparent);"
        "border-radius: 2px;"
        );
    layout->addWidget(line, 0, Qt::AlignCenter);

    layout->addSpacing(5);

    // Create Score Display Card
    QWidget *scoreCard = new QWidget();
    scoreCard->setFixedHeight(overlayHeight / 4);
    scoreCard->setStyleSheet(
        "QWidget {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #232837, stop:1 #1c212d);"
        "   border-radius: 55px;"
        "   border: 1px solid #2a2f3c;"
        "}"
        );

    QVBoxLayout *scoreLayout = new QVBoxLayout(scoreCard);
    scoreLayout->setContentsMargins(0, 15, 0, 15);
    scoreLayout->setSpacing(0);

    QLabel *scoreTitle = new QLabel("YOUR SCORE");
    scoreTitle->setAlignment(Qt::AlignCenter);
    scoreTitle->setStyleSheet("color: #8f9bb5; font-size: 13px; font-weight: 700; border: none; background: transparent;");

    QLabel *scoreValue = new QLabel(QString("%1").arg(m_score));
    scoreValue->setAlignment(Qt::AlignCenter);

    int scoreFontSize = qBound(32, overlayWidth / 8, 46);
    scoreValue->setStyleSheet(QString("color: #ffffff; font-size: %1px; font-weight: 800; border: none; background: transparent;").arg(scoreFontSize));

    scoreLayout->addWidget(scoreTitle);
    scoreLayout->addWidget(scoreValue);
    layout->addWidget(scoreCard);

    layout->addSpacing(15);

    // Create Action Buttons
    QPushButton *restartBtn = new QPushButton("PLAY AGAIN");
    restartBtn->setCursor(Qt::PointingHandCursor);
    restartBtn->setFixedHeight(overlayHeight / 8);
    restartBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2ecc71, stop:1 #27ae60);"
        "   color: white;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        );

    QPushButton *homeBtn = new QPushButton("HOME");
    homeBtn->setCursor(Qt::PointingHandCursor);
    homeBtn->setFixedHeight(overlayHeight / 8);
    homeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2f3545;"
        "   color: #e5e7eb;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #3f4559; }"
        );

    layout->addWidget(restartBtn);
    layout->addWidget(homeBtn);

    // Display Overlay With Fade Animation
    overlay->show();
    overlay->raise();

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(overlay, "windowOpacity");
    fadeAnim->setDuration(350);
    fadeAnim->setStartValue(0);
    fadeAnim->setEndValue(1);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // Connect Button Signals
    connect(restartBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        initGame(m_currentDifficulty);
    });

    connect(homeBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        this->hide();
        emit backToHome();
    });
}

/**
 * @brief Displays The Game Over Dialog With Score And Options
 */
void Snake::showGameOverDialog()
{
    // Calculate overlay size based on widget size
    int overlayWidth = qMin(400, width() - 100);
    int overlayHeight = qMin(480, height() - 100);

    // Create Overlay Widget
    QWidget *overlay = new QWidget(this);
    overlay->setFixedSize(overlayWidth, overlayHeight);
    overlay->move((width() - overlayWidth) / 2, (height() - overlayHeight) / 2);

    overlay->setStyleSheet(
        "background-color: #1a1e2b;"
        "border-radius: 40px;"
        "border: 1px solid #2a2f3c;"
        );

    // Add Shadow Effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(40);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 8);
    overlay->setGraphicsEffect(shadowEffect);

    // Setup Main Layout
    QVBoxLayout *layout = new QVBoxLayout(overlay);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 35, 40, 35);

    // Create Game Over Title
    QLabel *gameOverLabel = new QLabel("GAME OVER");
    gameOverLabel->setFixedHeight(overlayHeight / 8);
    gameOverLabel->setAlignment(Qt::AlignCenter);

    int titleFontSize = qBound(20, overlayWidth / 12, 28);
    gameOverLabel->setStyleSheet(
        QString("QLabel {"
                "   background-color: #2a2f3c;"
                "   color: #ff5e5e;"
                "   border-radius: %1px;"
                "   font-size: %2px;"
                "   font-weight: 900;"
                "   font-family: 'Arial Black';"
                "   letter-spacing: 2px;"
                "}").arg(overlayHeight / 16).arg(titleFontSize)
        );
    layout->addWidget(gameOverLabel);

    // Add Decorative Line
    QFrame *line = new QFrame();
    line->setFixedHeight(4);
    line->setFixedWidth(overlayWidth / 2);
    line->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 transparent, stop:0.5 #e74c3c, stop:1 transparent);"
        "border-radius: 2px;"
        );
    layout->addWidget(line, 0, Qt::AlignCenter);

    layout->addSpacing(5);

    // Create Score Display Card
    QWidget *scoreCard = new QWidget();
    scoreCard->setFixedHeight(overlayHeight / 4);
    scoreCard->setStyleSheet(
        "QWidget {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #232837, stop:1 #1c212d);"
        "   border-radius: 55px;"
        "   border: 1px solid #2a2f3c;"
        "}"
        );

    QVBoxLayout *scoreLayout = new QVBoxLayout(scoreCard);
    scoreLayout->setContentsMargins(0, 15, 0, 15);
    scoreLayout->setSpacing(0);

    QLabel *scoreTitle = new QLabel("YOUR SCORE");
    scoreTitle->setAlignment(Qt::AlignCenter);
    scoreTitle->setStyleSheet("color: #8f9bb5; font-size: 13px; font-weight: 700; border: none; background: transparent;");

    QLabel *scoreValue = new QLabel(QString("%1").arg(m_score));
    scoreValue->setAlignment(Qt::AlignCenter);

    int scoreFontSize = qBound(32, overlayWidth / 8, 46);
    scoreValue->setStyleSheet(QString("color: #ffffff; font-size: %1px; font-weight: 800; border: none; background: transparent;").arg(scoreFontSize));

    scoreLayout->addWidget(scoreTitle);
    scoreLayout->addWidget(scoreValue);
    layout->addWidget(scoreCard);

    layout->addSpacing(15);

    // Create Action Buttons
    QPushButton *restartBtn = new QPushButton("PLAY AGAIN");
    restartBtn->setCursor(Qt::PointingHandCursor);
    restartBtn->setFixedHeight(overlayHeight / 8);
    restartBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e74c3c, stop:1 #c0392b);"
        "   color: white;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        );

    QPushButton *homeBtn = new QPushButton("HOME");
    homeBtn->setCursor(Qt::PointingHandCursor);
    homeBtn->setFixedHeight(overlayHeight / 8);
    homeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2f3545;"
        "   color: #e5e7eb;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #3f4559; }"
        );

    layout->addWidget(restartBtn);
    layout->addWidget(homeBtn);

    // Display Overlay With Fade Animation
    overlay->show();
    overlay->raise();

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(overlay, "windowOpacity");
    fadeAnim->setDuration(350);
    fadeAnim->setStartValue(0);
    fadeAnim->setEndValue(1);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // Connect Button Signals
    connect(restartBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        initGame(m_currentDifficulty);
    });

    connect(homeBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        this->hide();
        emit backToHome();
    });
}

/**
 * @brief Show event override
 */
