#include <MainMenu.h>

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);
    MainMenu w;
    w.show();
    return a.exec();
}
