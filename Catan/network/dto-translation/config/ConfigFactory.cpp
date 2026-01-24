//
// Created by matija on 1/24/26.
//

#include "ConfigFactory.hpp"

BoardType ConfigFactory::fromProto(net::GameConfig::BoardType t) {
    switch (t) {
    case net::GameConfig::CLASSIC:  return BoardType::Classic;
    case net::GameConfig::EXTENDED: return BoardType::Extended;
    case net::GameConfig::CUSTOM:   return BoardType::Custom;
    }
    return BoardType::Classic;
}

GameConfig ConfigFactory::makeGameConfig(const net::LobbyInfo &proto)  {
    const net::GameConfig& cfgProto = proto.config();
    GameConfig cfg;

    cfg.numPlayers = cfgProto.num_players();
    cfg.winningVictoryPoints = cfgProto.winning_points();
    cfg.boardType = fromProto(cfgProto.board_type());
    cfg.numPlayers=cfgProto.num_players();
    cfg.players=std::vector(proto.names().names().begin(),proto.names().names().end());
    return cfg;
}