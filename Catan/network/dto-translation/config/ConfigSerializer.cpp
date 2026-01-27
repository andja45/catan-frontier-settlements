//
// Created by matija on 1/24/26.
//

#include "ConfigSerializer.hpp"


net::GameConfig::BoardType ConfigSerializer::toProto(BoardType t) {
    switch (t) {
    case BoardType::Classic:  return net::GameConfig::CLASSIC;
    case BoardType::Extended: return net::GameConfig::EXTENDED;
    case BoardType::Custom:   return net::GameConfig::CUSTOM;
    }
    return net::GameConfig::CLASSIC; // fallback
}

net::GameConfig ConfigSerializer::toProto(const GameConfig &cfg)  {
    net::GameConfig gcfg;

    gcfg.set_num_players(cfg.numPlayers);
    gcfg.set_winning_points(cfg.winningVictoryPoints);
    gcfg.set_board_type(toProto(cfg.boardType));

    gcfg.mutable_names()->Add(cfg.players.begin(),cfg.players.end());
    return gcfg;
}