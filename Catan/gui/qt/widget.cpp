#include <QVBoxLayout>

#include <widget.h>
#include "./ui/ui_widget.h"
#include <QRightSideBar.h>


#include "QBoard.h"
#include <board/Board.h>
#include <board/BoardFactory/AbstractBoardCreator.hpp>
#include <board/BoardFactory/RandomExtendedMapCreator.hpp>
#include <board/BoardFactory/RandomStandardMapCreator.hpp>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    board = (creator->getBoard());
    auto *qboard = new QBoard(this, board.get());

    auto *sidebar = new QRightSideBar(this);

    // Demo data
    sidebar->setBankSummary(BankSummary{19,19,19,19,19});
    sidebar->setPlayers({
        {"TupacShakur", 7, 0},
        {"Cavite", 5, 0},
        {"IdrisElba", 8, 0},
        {"AvocadoMan", 10, 0}
    });
    sidebar->addChatMessage("System", "Welcome!");
    sidebar->addChatMessage("AvocadoMan", "Hello.");

    // When user hits Send, append message (you can later forward to networking/model)
    connect(sidebar, &QRightSideBar::chatSendRequested, this, [sidebar](const QString& text){
        sidebar->addChatMessage("Me", text);
    });

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    layout->addWidget(qboard, 1);     // stretch: board grows
    layout->addWidget(sidebar, 0);    // fixed-ish width

    setLayout(layout);
}


Widget::~Widget()
{
    delete ui;
}
