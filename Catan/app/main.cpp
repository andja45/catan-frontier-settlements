#include <iostream>
#include <widget.h>

#include <QApplication>
#include <QMainWindow>
#include <board/BoardFactory/AbstractBoardCreator.hpp>
#include <board/BoardFactory/RandomExtendedMapCreator.hpp>
#include <board/BoardFactory/RandomStandardMapCreator.hpp>
#include <drawing/AsciiWindow.hpp>

#include "../gui/asciiview/board/BoardView.hpp"
#include "../gui/asciiview/board/TileView.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    auto board=(creator->getBoard());

    Widget w(nullptr,board.get());
    BoardView bw=BoardView(board.get(),{12,5});
    auto wnd=bw.toWindow();
    wnd.setBorder(true);
    wnd.setMargin({2,2});
    wnd.blit(std::cout);
    w.show();
    return a.exec();
}
