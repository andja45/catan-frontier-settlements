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

    cfg.setNumPlayers(cfgProto.num_players());
    cfg.setWinningPoints(cfgProto.winning_points());
    cfg.setBoardType(fromProto(cfgProto.board_type()));
    cfg.setPlayers(std::vector(cfgProto.names().begin(),cfgProto.names().end()));

    cfg.setName(cfgProto.game_name());
    return cfg;
}