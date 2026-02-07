//
// Created by matija on 1/27/26.
//

#include "LobbyApp.hpp"

#include "../common/ApplicationRoot.hpp"

LobbyApp::LobbyApp(ApplicationRoot *root, RoleType role)
    : QObject(nullptr), AbstractApplet(root){
    m_lobbyView=new LobbyView("Game Lobby",role);
    m_lobbyController=new LobbyController(m_lobbyView,m_root->getNetworkService()->getTransport());
    // create everything and connect signals!

    QObject::connect(m_lobbyController, &LobbyController::lobbyClosed, this, [this]() {
        m_root->showMainMenu();
    });
    QObject::connect(m_lobbyController, &LobbyController::gameStarted, this,
        [this](PlayerId myId, int seed, const GameConfig& config, Board* board) {
            std::unique_ptr<Board> ptr(board);
            m_root->startGame(config, std::move(ptr), myId, seed);
        });
}
