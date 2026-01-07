#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QFile>

#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    /*
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Catan – Main Menu");
    mainWindow.resize(400, 300);

    auto *central = new QWidget(&mainWindow);
    mainWindow.setCentralWidget(central);

    auto *layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);

    auto *btnHost    = new QPushButton("Host Game", central);
    auto *btnJoin    = new QPushButton("Join Game", central);
    auto *btnHistory = new QPushButton("Game History", central);

    btnHost->setFixedSize(200, 40);
    btnJoin->setFixedSize(200, 40);
    btnHistory->setFixedSize(200, 40);

    layout->addWidget(btnHost);
    layout->addWidget(btnJoin);
    layout->addWidget(btnHistory);

    mainWindow.show();
*/
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
    /*QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), QPixmap(":/images/catan2.png"));
    qDebug() << "PAINT EVENT" << size();


    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    static QPixmap bg(":/images/catan2.png");
    painter.drawPixmap(rect(), bg);
    painter.fillRect(rect(), Qt::red);

*/

    QPainter painter(this);

    QPixmap bg(":/images/catan2.png");
    qDebug() << "isNull =" << bg.isNull();

    painter.fillRect(rect(), Qt::red);
    painter.drawPixmap(rect(), bg);

    QFile f(":/images/catan2.png");
    qDebug() << "exists:" << f.exists();


}
