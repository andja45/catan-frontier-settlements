#include <QMainWindow>
#include <QVBoxLayout>

#include "widget.h"
#include "./ui/ui_widget.h"


#include "catanhexwidget.h"
#include <board/Board.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    auto *board = new CatanHexWidget(new Board(), this);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(board);
    setLayout(layout);
}

Widget::~Widget()
{
    delete ui;
}
