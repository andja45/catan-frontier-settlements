//
// Created by matija on 1/27/26.
//

#ifndef CATAN_NAVIGATOR_HPP
#define CATAN_NAVIGATOR_HPP
#include <memory>
#include <QTcpSocket>
#include <board/Board.h>
#include <common/NetworkService.hpp>
#include <model/GameConfig.h>
#include <screens/messageservice/PopupService.hpp>

#include "AbstractApplet.hpp"

class MenuApp;

class ApplicationRoot {
public:
    ApplicationRoot();

    void showMainMenu();
    void showLobbyHost();
    void showLobbyPlayer();
    void showHostPopup();
    void showJoinPopup();
    void startGame(const GameConfig& config, std::unique_ptr<Board> board, PlayerId myId, int seed);
    void showHistory();

    void closeApp();
    void startApp();

    NetworkService* getNetworkService() {return m_netService;}
    PopupService* getPopupService() {return m_msgService;}

    void clearPrevious();
    ~ApplicationRoot();


private:
    AbstractApplet* m_currentApplet=nullptr;

    MenuApp* m_menu=nullptr;
    NetworkService* m_netService=nullptr;
    PopupService* m_msgService=nullptr;

};

#endif //CATAN_NAVIGATOR_HPP