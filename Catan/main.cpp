#include "widget.h"

#include <QApplication>
#include "headers/Board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    auto board = new Board();
    return a.exec();
}
