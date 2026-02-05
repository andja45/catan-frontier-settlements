//
// Created by matija on 1/27/26.
//

#include "GameApp.hpp"

#include <GameNetworkAdapter.h>
#include <common/ApplicationRoot.hpp>
#include <game/GameController.hpp>

GameApp::GameApp(const GameConfig &config, std::unique_ptr<Board> board, PlayerId myId, int seed,
                 ApplicationRoot *root) : AbstractApplet(root) {
    auto boardPtr = board.get();
    m_gameSession = std::make_unique<GameSession>(config.getPlayerNames(),myId,seed,std::move(board), config.getPointsToWin(), config.getName());
    m_gameWindow = new GameWindow(boardPtr,m_gameSession->getPlayers(),myId,m_gameSession->getBank(),m_gameSession->getDice(), m_gameSession->getTrades(), nullptr);
    m_controller = new GameController(*m_gameSession,root->getNetworkService()->getTransport(),*m_gameWindow,nullptr);
    m_controller->connect(m_controller,&GameController::gameClosed,[this]() {
        m_root->showMainMenu();
    });
}
