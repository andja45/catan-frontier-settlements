#include "widget.h"

#include <QApplication>
#include "headers/Board.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    auto board = new Board();
    auto tile = board->getTiles()[10];
    std::cout << std::endl << tile->toString() << std::endl;
    std::cout << board->upLeft(tile)->toString() << std::endl;
    std::cout << board->upRight(tile)->toString() << std::endl;
    std::cout << board->right(tile)->toString() << std::endl;
    std::cout << board->downRight(tile)->toString() << std::endl;
    std::cout << board->downLeft(tile)->toString() << std::endl;
    std::cout << board->left(tile)->toString() << std::endl;
    return a.exec();
}
