#ifndef BASEGAMESCREEN_H
#define BASEGAMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "gamesizes.h"

class BaseGameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit BaseGameScreen(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~BaseGameScreen() {}

protected:
    QPushButton* createStyledButton(const QString &text,
                                    const QString &bgColor = "#3498db",
                                    const QString &hoverColor = "#2980b9") {
        QPushButton *btn = new QPushButton(text, this);
        btn->setFixedSize(GameSizes::controlButtonWidth(),
                          GameSizes::controlButtonHeight());
        btn->setFont(QFont("Arial", GameSizes::buttonFontSize(), QFont::Bold));
        btn->setStyleSheet(QString(
                               "QPushButton { background-color: %1; color: white; border: none; border-radius: 6px; }"
                               "QPushButton:hover { background-color: %2; }"
                               "QPushButton:disabled { background-color: #95a5a6; }"
                               ).arg(bgColor, hoverColor));
        return btn;
    }

    QLabel* createTimerLabel() {
        QLabel *label = new QLabel("00:00", this);
        label->setFont(QFont("Arial", GameSizes::titleFontSize(), QFont::Bold));
        label->setStyleSheet(
            "QLabel { color: #2c3e50; background-color: white; border-radius: 6px; padding: 5px; }"
            );
        label->setAlignment(Qt::AlignCenter);
        label->setFixedWidth(GameSizes::timerWidth());
        return label;
    }

    QLabel* createCaptionLabel(const QString &text) {
        QLabel *label = new QLabel(text, this);
        label->setFont(QFont("Arial", GameSizes::buttonFontSize()));
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color: #7f8c8d;");
        return label;
    }

    QPushButton* createBackButton() {
        QPushButton *btn = new QPushButton("←", this);
        btn->setFont(QFont("Arial", GameSizes::titleFontSize()));
        btn->setFixedSize(40, 40);
        btn->setStyleSheet(
            "QPushButton { background-color: transparent; color: #2c3e50; border: none; }"
            "QPushButton:hover { color: #3498db; }"
            );
        return btn;
    }

    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        if (overlay) {
            overlay->setGeometry(rect());
        }
    }

    QWidget *overlay = nullptr;

signals:
    void backToHomeClicked();
};

#endif // BASEGAMESCREEN_H
