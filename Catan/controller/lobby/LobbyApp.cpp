//
// Created by matija on 1/27/26.
//

#include "LobbyApp.hpp"

#include "../common/ApplicationRoot.hpp"

LobbyApp::LobbyApp(ApplicationRoot *root, RoleType role)
    : AbstractApplet(root){
    m_lobbyView=new LobbyView("Game Lobby",role);
    m_lobbyController=new LobbyController(m_lobbyView,m_root->getNetworkService()->getTransport());
    // create everything and connect signals!

    m_lobbyController->connect(m_lobbyController,&LobbyController::lobbyClosed,[this]() {
       m_root->showMainMenu();
    });
    m_lobbyController->connect(m_lobbyController,&LobbyController::gameStarted,[this](const GameConfig& config, Board* board, PlayerId myId, int seed) {
        std::unique_ptr<Board> ptr(board);
        m_root->startGame(config,std::move(ptr),myId,seed);
    });
    m_lobbyController->connect(m_lobbyController,&LobbyController::lobbyClosed,[this]() {
         m_root->showMainMenu();
    });
}



