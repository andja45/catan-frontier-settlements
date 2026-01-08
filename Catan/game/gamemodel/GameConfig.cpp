//
// Created by andja on 7.1.26..
//

#include "GameConfig.h"
#include <cassert>

GameConfig::GameConfig(int numPlayers,
                       int winningPoints,
                       uint32_t seed,
                       std::vector<PlayerConfig> players)
    : numPlayers(numPlayers),
      winningVictoryPoints(winningPoints),
      randomSeed(seed),
      players(std::move(players))
{
    assert(this->numPlayers == static_cast<int>(this->players.size()));
}

const PlayerConfig& GameConfig::player(const PlayerId id) const {
    assert(id >= 0 && id < static_cast<PlayerId>(players.size())); // they will get next id svailable, client-host having id 0
    return players[id];
}