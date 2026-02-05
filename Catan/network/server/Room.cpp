//
// Created by matija on 1/24/26.
//

#include "Room.hpp"
#include <random>
#include <ServerTypes.hpp>
#include <board/BoardFactory.hpp>
#include <board/BoardSerializer.hpp>
#include <config/ConfigFactory.hpp>
#include <config/ConfigSerializer.hpp>
#include <move/MoveProtoFactory.h>

Room::Room(const std::string &name, ClientConnection *host) {
    m_name  =name;
    m_host  =host;
    m_config=std::make_unique<GameConfig>();
    m_seed = std::random_device{}();
    m_config->setNumPlayers(4); // so host can join!

}

void Room::addPlayer(ClientConnection *c, std::string name) {
    if (isFull()) {
        kickPlayer(c);
        return;
    }
    m_players.push_back(c);

    m_config->addPlayer(name);
    broadcastConfig();
}

void Room::removePlayer(ClientConnection *c) {
    for (auto it =m_players.begin(); it !=m_players.end(); ++it) {
        if (*it == c) {
            m_players.erase(it);
            break;
        }
    }
    c->setRoomId(std::nullopt);
    c->setHost(false);
    c->setStatus(ClientStatus::Connected);

    m_config->removePlayer(c->name());
    broadcastConfig();
}

void Room::kickPlayer(ClientConnection *c) {
    if (m_state==RoomState::InGame) {
        net::Move move; // we kick player (force them to leave kindly)
        move.set_player_id(c->playerId());
        move.set_type(net::Move_MoveType_PlayerLeave);
        broadcastMove(move);
    }
    removePlayer(c);
}

void Room::processMove(ClientConnection *c,const net::Move & move) {
    if (m_state != RoomState::InGame){kickPlayer(c);return;}
    if (c->playerId()!=move.player_id()) {
        kickPlayer(c);
        return;
    }

    auto gameMove=MoveProtoFactory::fromProto(move);
    bool valid=gameMove->isValid(*m_session.get());
    if (!valid) {
        kickPlayer(c);
        return;
    }
    m_session->applyMove(*gameMove.get());

    broadcastMove(move);
}

void Room::processMessage(ClientConnection *, const std::string &msg) {
    auto env=net::Envelope();
    env.set_msg_type(net::MsgType::MSG_CHAT);
    env.mutable_chat()->set_message(msg);
    broadcast(env);
}

void Room::processConfig(ClientConnection *c, const net::GameConfig &config) {
    if (m_state != RoomState::Lobby) {kickPlayer(c); return;}
    if (!c->isHost()) {kickPlayer(c); return;}

    m_config->copySettingsFromConfig(ConfigFactory::fromProto(config));
    broadcastConfig();
}

void Room::processStartRequest(ClientConnection *c, const net::StartGameRequest &request) {
    if (m_state != RoomState::Lobby) {kickPlayer(c);return;}
    if (!c->isHost()){kickPlayer(c);return;}

    net::BoardInfo protoBoard=request.board_info();

    auto gameBoard=BoardFactory::fromProto(protoBoard);
    m_session=std::make_unique<GameSession>(m_config->getPlayerNames(),-1,m_seed,std::move(gameBoard),m_config->getPointsToWin(),m_name);


    for (int i=0;i<m_players.size();++i) {
        auto p=m_players[i];
        p->setStatus(ClientStatus::WaitingForGame);
        p->setPlayerId(i);
    }

    broadcastStartGame();

    m_state=RoomState::Lobby; // we are waiting for all players to ack
    m_config=nullptr;

}

void Room::broadcast(const net::Envelope & env) {
    for (auto& p : m_players) {
        p->send(env);
    }
}

bool Room::isEmpty() const {
    return m_players.empty();
}

bool Room::isFull() const {
    return m_players.size() >= m_config->getMaxPlayers();
}

void Room::broadcastConfig() {
    auto netConfig=ConfigSerializer::toProto(*m_config);
    net::Envelope env;
    env.set_msg_type(net::MsgType::MSG_SETUP);
    env.mutable_setup()->set_type(net::Setup_MessageType_GAME_CONFIG);
    *env.mutable_setup()->mutable_game_config()=netConfig;
    broadcast(env);
}

void Room::broadcastMove(const net::Move move) {
    net::Envelope env;
    env.set_msg_type(net::MsgType::MSG_MOVE);
    env.mutable_move()->CopyFrom(move);
    broadcast(env);
}

void Room::broadcastStartGame() {
    for (int i =0; i<m_players.size(); ++i) {
        net::Envelope env;
        env.set_msg_type(net::MsgType::MSG_SETUP);
        env.mutable_setup()->set_type(net::Setup_MessageType_GAME_START);

        auto gameStartData=env.mutable_setup()->mutable_game_start();
        gameStartData->set_player_id(i);
        gameStartData->set_seed(m_seed);
        *gameStartData->mutable_board_info()=BoardSerializer::toProto(m_session->board());
        *gameStartData->mutable_game_config()=ConfigSerializer::toProto(*m_config);

        m_players[i]->send(env);
    }
}

void Room::setHost(ClientConnection *host) {
    m_host=host;
}

void Room::kickInactivePlayers() {
    for (auto it=m_players.begin();it!=m_players.end();) {
        if (!(*it)->wasActive()) {
            kickPlayer(*it);
        } else {
            ++it;
        }
    }
    for (auto p:m_players) {
        p->resetActive();
    }
}

void Room::kickEveryone() {
    for (auto p:m_players) {
        kickPlayer(p);
    }
}

void Room::setPlayerReady(ClientConnection *c) {
    c->setStatus(ClientStatus::InGame); // player has opened game but we may not start yet, we are waiting for other players
    if (checkStartGame()) {
        lobbyToGame();
    }
}

bool Room::checkStartGame() {
    for (auto p:m_players) {
        if (p->status()!=ClientStatus::InGame) return false;
    }
    return true;
}

void Room::lobbyToGame() {
    m_state=RoomState::InGame;
    for (auto p:m_players) {
        p->ack();
    }
}

void Room::processError(ClientConnection *client, const std::string &error) {
    kickPlayer(client);
}
