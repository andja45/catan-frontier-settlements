//
// Created by andja on 7.1.26.
//

#ifndef CATAN_GAMECONFIG_H
#define CATAN_GAMECONFIG_H

#include <string>
#include <vector>
#include <cstdint>
#include "types/TypeAliases.h"

struct PlayerConfig {
    PlayerId id;
    std::string name;
};

class GameConfig { // class used to pass game configuration from client-host to other clients for game setup and initialization
public:
    int numPlayers = 0;
    int winningVictoryPoints = 10;
    uint32_t randomSeed = 0; // for random sync
    // board type?
    std::vector<PlayerConfig> players;
public:
    GameConfig() = default;

    GameConfig(int numPlayers,
               int winningPoints,
               uint32_t seed,
               std::vector<PlayerConfig> players);

    const PlayerConfig& player(PlayerId id) const;
};

#endif //CATAN_GAMECONFIG_H