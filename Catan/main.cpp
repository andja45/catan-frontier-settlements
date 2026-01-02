#include "widget.h"

#include <QApplication>
#include <QMainWindow>

#include "catanhexwidget.h"
#include "headers/Board/Board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    auto board = new Board();

    QMainWindow win;
    auto *w = new CatanHexWidget(board);
    win.setCentralWidget(w);
    win.resize(900, 900);
    win.show();

    return a.exec();
}
