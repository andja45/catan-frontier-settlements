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