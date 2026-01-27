//
// Created by andja on 7.1.26..
//

#include "GameConfig.h"
#include <cassert>

GameConfig::GameConfig(int numPlayers,
                       int winningPoints,
                       std::vector<std::string> players)
    : numPlayers(numPlayers),
      winningVictoryPoints(winningPoints),
      players(std::move(players))
{
    assert(this->numPlayers == static_cast<int>(this->players.size()));
}

const std::string& GameConfig::player(const PlayerId id) const {
    assert(id >= 0 && id < static_cast<PlayerId>(players.size())); // they will get next id available, client-host having id 0
    return players[id];
}

void GameConfig::addPlayer(const std::string &name) {
    players.push_back(name);
}

void GameConfig::removePlayer(std::string name) {
    for (auto it =players.begin(); it !=players.end(); ++it) {
        if (*it == name) {
            players.erase(it);
            return;
        }
    }
}

void GameConfig::copySettingFromConfig(const GameConfig &config) {
    numPlayers = config.numPlayers;
    winningVictoryPoints = config.winningVictoryPoints;
    boardType=config.boardType;
}
