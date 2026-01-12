#include <widget.h>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);
    Widget w;
    w.show();
    return a.exec();
}
