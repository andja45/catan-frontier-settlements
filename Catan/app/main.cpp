#include <widget.h>

#include <QApplication>
#include <QMainWindow>
#include <board/BoardFactory/AbstractBoardCreator.hpp>
#include <board/BoardFactory/RandomStandardSmallCreator.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
