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

    auto *board = new QBoard(new Board(), this);
    auto *menu = new QMenu(this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(board);
    layout->addWidget(menu);
}

Widget::~Widget()
{
    delete ui;
}
