//
// Created by matija on 1/24/26.
//

#include "Room.hpp"
#include <random>
#include <board/BoardFactory.hpp>
#include <board/BoardSerializer.hpp>
#include <config/ConfigFactory.hpp>
#include <config/ConfigSerializer.hpp>
#include <move/MoveProtoFactory.h>

Room::Room(const std::string &name, ClientConnection *host) {
    m_name  =name;
    m_host  =host;
    auto rd =std::random_device{};
    m_d6    =std::uniform_int_distribution<int>{1,6};
    m_seed  =static_cast<int> (rd());
    m_rng   =std::mt19937{static_cast<unsigned long>(m_seed)};
    m_config=std::make_unique<GameConfig>();
}

void Room::addPlayer(ClientConnection *c, std::string name) {
    if (isFull()) return; // TODO
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
    m_config->removePlayer(c->name());
}

void Room::kickPlayer(ClientConnection *client_connection) {
    net::Move move; // we kick player (force them to leave kindly)
    move.set_player_id(client_connection->playerId());
    move.set_type(net::Move_MoveType_PlayerLeave);
    broadcastMove(move);
    client_connection->disconnect(); //!
}

void Room::processMove(ClientConnection *c,net::Move & move) {

    if (move.type() == net::Move_MoveType_RollDice) {
        processDice(move);
    } else if (move.type() == net::Move_MoveType_PlayerTradeRequest) {
        processTradeRequest(move);
    }

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
    gameMove->apply(*m_session.get());

    broadcastMove(move);
}

void Room::processMessage(ClientConnection *, const std::string &msg) {
    auto env=net::Envelope();
    env.set_msg_type(net::MsgType::MSG_CHAT);
    env.mutable_chat()->set_message(msg);
    broadcast(env);
}

void Room::processConfig(ClientConnection *c, const net::GameConfig &config) {
    if (m_state != RoomState::Lobby) {kickPlayer(c); return;} //!
    if (!c->isHost()) {kickPlayer(c); return;} //!
    m_config->copySettingFromConfig(ConfigFactory::fromProto(config));
}

void Room::processStartRequest(ClientConnection *c, const net::StartGameRequest &request) {
    if (m_state != RoomState::Lobby) {kickPlayer(c);return;} //!
    if (!c->isHost()){kickPlayer(c);return;}


    net::BoardInfo protoBoard=request.board_info();

    auto gameBoard=BoardFactory::fromProto(protoBoard);
    m_session=std::make_unique<GameSession>(m_config->getPlayerNames(),-1,m_seed,std::move(gameBoard),m_config->getPointsToWin(),m_name);


    m_config=nullptr;
    m_state=RoomState::InGame;


    broadcastStartGame();
}

void Room::broadcast(const net::Envelope & env) {
    for (auto& p : m_players) {
        p->send(env);
    }
}

void Room::processDice(net::Move& move) {
    int dice1=m_d6(m_rng);
    int dice2=m_d6(m_rng);
    move.mutable_roll_dice()->set_dice_1(dice1);
    move.mutable_roll_dice()->set_dice_2(dice2);
}

void Room::processTradeRequest(net::Move& move) {
    int tradeId=m_currTradeId++;
    move.mutable_trade_request()->set_trade_request_id(tradeId);
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
    *env.mutable_setup()->mutable_game_config()=netConfig;
    broadcast(env);
}

void Room::broadcastMove(const net::Move) {
    net::Envelope env;
    env.set_msg_type(net::MsgType::MSG_MOVE);
    broadcast(env);
}

void Room::broadcastStartGame() {
    for (int i =0; i<m_players.size(); ++i) {
        net::Envelope env;
        env.set_msg_type(net::MsgType::MSG_SETUP);
        env.mutable_setup()->set_type(net::Setup_MessageType_GAME_START);

        auto gameStartData=env.mutable_setup()->mutable_game_start();
        gameStartData->set_seed(m_seed);
        gameStartData->set_player_id(i);
        gameStartData->set_seed(m_seed);
        *gameStartData->mutable_board_info()=BoardSerializer::toProto(m_session->board());
        *gameStartData->mutable_game_config()=ConfigSerializer::toProto(*m_config);

        m_players[i]->send(env);
    }
}
