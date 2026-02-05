#include <iostream>
#include <../gui/qt/screens/mainmenu/MainMenu.h>

#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <board/BoardView.hpp>
#include <board/factory/AbstractBoardCreator.hpp>
#include <board/factory/RandomExtendedMapCreator.hpp>
#include <QStyleFactory>
#include <QPalette>
#include <QApplication>
#include <qdir.h>
#include <qstyle.h>
#include <screens/connect/HostJoinView.h>
#include <screens/game/GameWindow.h>
#include <screens/history/GameHistory.h>
#include <screens/lobby/LobbyView.h>
#include <QShortcut>
#include <QKeySequence>
#include <common/ApplicationRoot.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);

    // set up uniform look
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, Qt::white);
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240,240,240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(245,245,245));
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Highlight, QColor(0,120,215)); // standard blue
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127,127,127));
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127,127,127));
    a.setPalette(lightPalette);

    QFile file(":/styles/catanbuttons.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());


    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    auto board=(creator->getBoard());
    auto asciiBoard=BoardView(board.get(),BoardTheme(),{12,5},{3,3});
    asciiBoard.drawBoard(std::cout);

    ApplicationRoot app;
    app.showMainMenu();

    return a.exec();
}
