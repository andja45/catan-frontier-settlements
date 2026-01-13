#include <QMainWindow>
#include <QVBoxLayout>

#include "widget.h"
#include "./ui/ui_widget.h"


#include "QBoard.h"
#include <board/Board.h>
#include <board/BoardFactory/AbstractBoardCreator.hpp>
#include <board/BoardFactory/RandomStandardMapCreator.hpp>
#include <board/BoardFactory/RandomExtendedMapCreator.hpp>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    board=(creator->getBoard());
    auto *qboard = new QBoard(this,board.get());

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(qboard);
    setLayout(layout);
}


Widget::~Widget()
{
    delete ui;
}
