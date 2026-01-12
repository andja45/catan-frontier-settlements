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
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);

    auto *btnHost    = new QPushButton("Host Game", this);
    auto *btnJoin    = new QPushButton("Join Game", this);
    auto *btnHistory = new QPushButton("Game History", this);

    btnHost->setFixedSize(200, 40);
    btnJoin->setFixedSize(200, 40);
    btnHistory->setFixedSize(200, 40);

    layout->addWidget(btnHost);
    layout->addWidget(btnJoin);
    layout->addWidget(btnHistory);

    update();
}

void MainMenu::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    QPixmap bg(":/images/catan.png");
    qDebug() << "isNull =" << bg.isNull();

    painter.fillRect(rect(), Qt::red);
    painter.drawPixmap(rect(), bg);

    QFile f(":/images/catan.png");
    qDebug() << "exists:" << f.exists();


}
