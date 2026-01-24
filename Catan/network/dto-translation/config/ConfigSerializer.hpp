//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CONFIGSERIALIZER_HPP
#define CATAN_CONFIGSERIALIZER_HPP

#include <model/GameConfig.h>
#include <types/BoardType.hpp>

#include "ProtoHeaders.hpp"

class ConfigSerializer {
    static net::GameConfig::BoardType toProto(BoardType t);
    net::LobbyInfo serializeGameConfig(const GameConfig& cfg);

};

#endif //CATAN_CONFIGSERIALIZER_HPP