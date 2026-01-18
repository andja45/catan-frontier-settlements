#include <../gui/qt/screens/game/widget.h>

#include <QApplication>
#include <QMainWindow>
#include <board/factory/AbstractBoardCreator.hpp>
#include <board/factory/RandomStandardMapCreator.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
