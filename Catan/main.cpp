#include "widget.h"

#include <QApplication>
#include <QMainWindow>

#include "catanhexwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    QMainWindow win;
    auto *w = new CatanHexWidget();
    win.setCentralWidget(w);
    win.resize(900, 900);
    win.show();

    return a.exec();
}
