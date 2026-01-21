#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <board/BoardView.hpp>
#include <board/factory/AbstractBoardCreator.hpp>
#include <board/factory/RandomExtendedMapCreator.hpp>
#include <screens/game/widget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    auto board=(creator->getBoard());

    Widget w(nullptr,board.get());
    BoardView bw=BoardView(board.get(),BoardTheme(),{12,5},{3,3});
    bw.drawBoard(std::cout);
    w.show();
    return a.exec();
}
