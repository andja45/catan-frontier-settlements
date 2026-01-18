#include <iostream>
#include <widget.h>

#include <QApplication>
#include <QMainWindow>
#include <board/BoardFactory/AbstractBoardCreator.hpp>
#include <board/BoardFactory/RandomExtendedMapCreator.hpp>
#include <board/BoardFactory/RandomStandardMapCreator.hpp>

#include "../gui/asciiview/board/BoardView.hpp"
#include "../gui/asciiview/board/TileView.hpp"

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
