//
// Created by andja on 7.1.26.
//

#ifndef CATAN_GAMECONFIG_H
#define CATAN_GAMECONFIG_H

#include <string>
#include <vector>
#include <cstdint>
#include <types/BoardType.hpp>
#include "types/TypeAliases.h"


class GameConfig { // class used to pass game configuration from client-host to other clients for game setup and initialization
public:
    int numPlayers = 0;
    int winningVictoryPoints = 10;
    BoardType boardType= BoardType::Classic;
    std::vector<std::string> players;
public:
    GameConfig() = default;

    GameConfig(int numPlayers,
               int winningPoints,
               std::vector<std::string> players);

    const std::string& player(PlayerId id) const;
    void addPlayer(const std::string &name);
    void removePlayer(std::string name);
    void copySettingFromConfig(const GameConfig& config);
    int getMaxPlayers() const {return numPlayers;}
    std::vector<std::string> getPlayerNames() const {return players;}
    int getPointsToWin() const {return winningVictoryPoints;}
};

#endif //CATAN_GAMECONFIG_H