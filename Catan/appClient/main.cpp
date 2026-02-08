#include <iostream>
#include <../gui/qt/screens/mainmenu/MainMenu.h>

#include <QApplication>
#include <board/BoardView.hpp>
#include <board/factory/AbstractBoardCreator.hpp>
#include <board/factory/RandomExtendedMapCreator.hpp>
#include <common/ApplicationRoot.hpp>
#include <drawing/AsciiWindow.hpp>
#include <common/style/QtAppStyle.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setQuitOnLastWindowClosed(false);
    Q_INIT_RESOURCE(resources);

    catan::gui::qt::applyDefaultQtStyle(a);
    QCoreApplication::setApplicationName("Catan");

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    auto board=(creator->getBoard());
    BoardView bw=BoardView(board.get(),{12,5});
    auto wnd=bw.toWindow();
    wnd.setBorder(true);
    wnd.setMargin({2,2});
    wnd.blit(std::cout);


    ApplicationRoot app;
    app.showMainMenu();

    return a.exec();
}
