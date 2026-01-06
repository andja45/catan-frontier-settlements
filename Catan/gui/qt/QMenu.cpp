#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "QMenu.h"

QMenu::QMenu(QWidget *parent) : QWidget(parent)
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
}
