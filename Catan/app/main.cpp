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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resources);
    MainMenu menu;

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

    menu.show();

    AbstractBoardCreator *creator = new RandomExtendedMapCreator();
    auto board=(creator->getBoard());



    // TODO DEBUG ONLY

    Trade trade1(0, {{ResourceType::Wood, 2}, {ResourceType::Brick, 1}}, {{ResourceType::Wool, 1}});
    Trade trade2(1, {{ResourceType::Ore, 3}}, {{ResourceType::Wheat, 2}});
    Trade trade3(2, {{ResourceType::Wheat, 1}, {ResourceType::Wood, 1}}, {{ResourceType::Wheat, 1}, {ResourceType::Wood, 1},{ResourceType::Brick, 2},{ResourceType::Ore, 1}});
    Trade trade4(2, {{ResourceType::Wool, 2}}, {{ResourceType::Ore, 1}, {ResourceType::Wheat, 1}});

    trade3.markResponded(0);
    trade3.markResponded(1);

    trade4.markResponded(0);
    trade4.markResponded(1);
    trade4.markResponded(2);
    trade4.markResponded(3);

    std::unordered_map<TradeId,Trade> trades={{0,trade1},{2,trade3},{3,trade4},{0,trade2}};


    std::vector<Player*> m_players;
    m_players.push_back(new Player(0, "Lazar"));
    m_players.push_back(new Player(1, "Jovana"));
    auto* andjela=new Player(2, "Andjela");
    andjela->addResources({{ResourceType::Wood,5},{ResourceType::Wool,2},{ResourceType::Brick,7}});
    andjela->addDevCard(DevCardType::Monopoly);
    andjela->addDevCard(DevCardType::YearOfPlenty);
    m_players.push_back(andjela);
    m_players.push_back(new Player(3, "Marko"));
    Bank* m_bank=new Bank();

    GameWindow w(board.get(),m_players,2,m_bank,&trades,nullptr);

    HostJoinView* host=new HostJoinView(RoleType::Host);
    HostJoinView* join=new HostJoinView(RoleType::Join);
    host->show();

    LobbyView* lobby1=new LobbyView("Najbolji catan",RoleType::Host,nullptr);
    LobbyView* lobby2=new LobbyView("Najbolji catan",RoleType::Join,nullptr);
    lobby1->show();

    GameHistory* gameHist=new GameHistory();
    gameHist->show();

    BoardView bw=BoardView(board.get(),BoardTheme(),{12,5},{3,3});
    bw.drawBoard(std::cout);
    w.show();


    auto* shortcut = new QShortcut(QKeySequence(Qt::Key_D), &w);
    w.connect(shortcut, &QShortcut::activated, w.getActionManager(),&ActionManager::openDiscardPopup);

    auto* shortcut2 = new QShortcut(QKeySequence(Qt::Key_S), &w);

    w.connect(shortcut2, &QShortcut::activated, w.getActionManager(),[ &w]() {
        w.getActionManager()->openStealCardPopup({1,2,3});
    });


    return a.exec();
}
