//
// Created by matija on 1/24/26.
//

#include "LobbyController.hpp"

#include <board/BoardFactory.hpp>
#include <board/factory/LoadFromFileCreator.hpp>
#include <board/factory/RandomExtendedMapCreator.hpp>
#include <board/factory/RandomStandardMapCreator.hpp>

LobbyController::LobbyController(LobbyView *view, NetworkTransport *transport): QObject(nullptr), m_view(view) {
    m_transport = new LobbyNetworkAdapter(this);
    m_transport->setTransport(transport);

    connect(m_view,&LobbyView::startGameRequested,this,&LobbyController::onGameRequested);
    connect(m_view,&LobbyView::configChanged,[this](const GameConfig& config){
        m_transport->sendConfig(config);
    });
    connect(m_transport,&LobbyNetworkAdapter::configReceived, m_view,&LobbyView::onConfigChanged);
    connect(m_transport,&LobbyNetworkAdapter::gameStarted,this,&LobbyController::onGameStarted);
    connect(m_view,&LobbyView::close,this,[this]() {
        m_transport->sendLeave();
        emit lobbyClosed();
    });
}

void LobbyController::onGameRequested(const GameConfig &config, const std::string& boardPath) {
    Board* board;
    if (config.boardType==BoardType::Custom) {
        board=(new LoadFromFileCreator(boardPath))->getBoard().get();
    }
    else if (config.boardType==BoardType::Classic){
        board=(new RandomStandardMapCreator())->getBoard().get();
    }
    else {
        board=(new RandomExtendedMapCreator())->getBoard().get();
    }
    m_transport->sendStartRequest(*board);
}

void LobbyController::onGameStarted(PlayerId myId, int32_t gameSeed, const GameConfig &config, Board *board) {
    emit gameStarted(config,board,myId,gameSeed);
}
