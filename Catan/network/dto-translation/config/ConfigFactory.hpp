//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CONFIGFACTORY_HPP
#define CATAN_CONFIGFACTORY_HPP

#include <model/GameConfig.h>
#include <types/BoardType.hpp>

#include "ProtoHeaders.hpp"

class ConfigFactory {
    static BoardType fromProto(net::GameConfig::BoardType t);
    static GameConfig makeGameConfig(const net::LobbyInfo& proto);

};

#endif //CATAN_CONFIGFACTORY_HPP