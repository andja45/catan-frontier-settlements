#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QtMultimedia/QSoundEffect>
#include <screens/mainmenu/MainMenu.h>
#include "../../common/audio/AudioManager.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFixedSize(680, 570);

    // popup at center of screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    const int x = screenGeometry.x() + (screenGeometry.width()  - this->width())  / 2;
    const int y = screenGeometry.y() + (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    auto *btnHost    = new QPushButton("Host Game", this);
    auto *btnJoin    = new QPushButton("Join Game", this);
    auto *btnHistory = new QPushButton("Game History", this);

    btnHost->setProperty("catan", true);
    btnJoin->setProperty("catan", true);
    btnHistory->setProperty("catan", true);

    QFont font("Baloo 2", 20, QFont::Bold);
    btnHost->setFont(font);
    btnJoin->setFont(font);
    btnHistory->setFont(font);

    btnHost->setFixedSize(220, 50);
    btnJoin->setFixedSize(220, 50);
    btnHistory->setFixedSize(220, 50);

    btnHost->setProperty("catan", true);
    btnJoin->setProperty("catan", true);
    btnHistory->setProperty("catan", true);

    layout->addWidget(btnHost);
    layout->addWidget(btnJoin);
    layout->addWidget(btnHistory);

    auto playClick = []() { AudioManager::instance().play(AudioManager::Sfx::Click); };

    connect(btnHost, &QPushButton::pressed, this, [playClick, this](){playClick(); emit hostGame();});
    connect(btnJoin, &QPushButton::pressed, this, [playClick, this](){playClick(); emit hostGame();});
    connect(btnHistory, &QPushButton::pressed, this, [playClick, this](){playClick(); emit hostGame();});

    update();
}

void MainMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap bg(":/backgrounds/catan.png");

    painter.fillRect(rect(), Qt::red);
    painter.drawPixmap(rect(), bg);
}

void MainMenu::closeEvent(QCloseEvent *event) {
    emit quit();
    QWidget::closeEvent(event);
}
