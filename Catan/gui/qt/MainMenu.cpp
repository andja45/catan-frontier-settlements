#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QFile>
#include <QResource>

#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFixedSize(680, 570);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    auto *btnHost    = new QPushButton("Host Game", this);
    auto *btnJoin    = new QPushButton("Join Game", this);
    auto *btnHistory = new QPushButton("Game History", this);

    QFile file(":/styles/catanbuttons.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());

    QFont font("Baloo 2", 20, QFont::Bold);
    btnHost->setFont(font);
    btnJoin->setFont(font);
    btnHistory->setFont(font);

    btnHost->setFixedSize(220, 50);
    btnJoin->setFixedSize(220, 50);
    btnHistory->setFixedSize(220, 50);

    layout->addWidget(btnHost);
    layout->addWidget(btnJoin);
    layout->addWidget(btnHistory);

    update();
}

void MainMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap bg(":/images/catan.png");

    painter.fillRect(rect(), Qt::red);
    painter.drawPixmap(rect(), bg);
}
