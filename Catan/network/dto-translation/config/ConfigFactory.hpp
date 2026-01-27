//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CONFIGFACTORY_HPP
#define CATAN_CONFIGFACTORY_HPP

#include <model/GameConfig.h>
#include <types/BoardType.hpp>

#include "ProtoHeaders.hpp"

class ConfigFactory {
public:
    static BoardType fromProto(net::GameConfig::BoardType t);
    static GameConfig fromProto(const net::GameConfig& proto);

};

#endif //CATAN_CONFIGFACTORY_HPP