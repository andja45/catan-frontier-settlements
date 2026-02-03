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

    void closeApp();

    NetworkService* getNetworkService() {return m_service;}

    void clearPrevious();

private:
    AbstractApplet* m_currentApplet;

    MenuApp* m_menu;
    NetworkService* m_service;

    ~ApplicationRoot();
};

#endif //CATAN_NAVIGATOR_HPP