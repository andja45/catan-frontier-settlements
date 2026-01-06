#include <QMainWindow>
#include <QVBoxLayout>

#include "widget.h"
#include "./ui/ui_widget.h"


#include "QBoard.h"
#include "QMenu"
#include <board/Board.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

<<<<<<< Updated upstream
    auto *board = new QBoard(new Board(), this);
=======
    //auto *board = new QBoard(this);
    auto *menu = new QMenu(this);
>>>>>>> Stashed changes

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    //layout->addWidget(board);



    layout->addWidget(menu);

    setLayout(layout);
}

Widget::~Widget()
{
    delete ui;
}
