#include <QMainWindow>
#include <QVBoxLayout>

#include "widget.h"
#include "./ui/ui_widget.h"


//#include "QBoard.h"
#include "MainMenu.h"
#include <board/Board.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    // Postavlja sliku da se rasteže preko celog prozora (border-image)
    this->setStyleSheet("border-image: url(:/images/catan2.png);");


    //auto *board = new QBoard(new Board(), this);
    auto *menu = new MainMenu(this);

    auto *layout = new QVBoxLayout(this);
    //layout->addWidget(board);
    layout->addWidget(menu);
}

Widget::~Widget()
{
    delete ui;
}
