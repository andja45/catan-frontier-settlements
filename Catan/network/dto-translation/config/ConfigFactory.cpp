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

GameConfig ConfigFactory::fromProto(const net::GameConfig &cfgProto)  {
    GameConfig cfg;

    cfg.numPlayers = cfgProto.num_players();
    cfg.winningVictoryPoints = cfgProto.winning_points();
    cfg.boardType = fromProto(cfgProto.board_type());
    cfg.numPlayers=cfgProto.num_players();
    cfg.players=std::vector(cfgProto.names().begin(),cfgProto.names().end());
    return cfg;
}