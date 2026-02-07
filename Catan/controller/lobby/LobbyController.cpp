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
    connect(m_transport,&LobbyNetworkAdapter::gameStarted,this,[this](PlayerId myId, int seed,const GameConfig& config, Board* board) {
        emit gameStarted(myId,seed,config,board);
    });
    connect(m_view,&LobbyView::closed,this,[this]() {
        m_transport->sendLeave();
        emit lobbyClosed();
    });
    connect(m_transport,&LobbyNetworkAdapter::disconnected,this,[this]() {
        emit lobbyClosed();
    }); //popup service?
    connect(m_transport,&LobbyNetworkAdapter::errored,this,[this](const std::string& error) {
        m_transport->sendLeave();
        emit lobbyClosed();
    });

    m_transport->sendJoined(); // we send ack to finalize us joining the room, awaiting for config from server
}

void LobbyController::onGameRequested(const GameConfig &config, const std::string& boardPath) {
    Board* board;
    std::unique_ptr<AbstractBoardCreator> factory;
    if (config.getBoardType()==BoardType::Custom) {
        factory=std::make_unique<LoadFromFileCreator>(boardPath);
    }
    else if (config.getBoardType()==BoardType::Classic){
        factory=std::make_unique<RandomStandardMapCreator>();
    }
    else {
        factory=std::make_unique<RandomExtendedMapCreator>();
    }
        board=factory->getBoard().release();

    m_transport->sendStartRequest(*board);
}

