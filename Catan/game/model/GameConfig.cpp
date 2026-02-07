//
// Created by andja on 7.1.26..
//

#include "GameConfig.h"
#include <cassert>

GameConfig::GameConfig(int numPlayers,
                       int winningPoints,
                       std::vector<std::string> players)
    : m_numPlayers(numPlayers),
      m_winningVictoryPoints(winningPoints),
      m_players(std::move(players))
{
    assert(this->m_numPlayers == static_cast<int>(this->m_players.size()));
}

const std::string& GameConfig::player(const PlayerId id) const {
    assert(id >= 0 && id < static_cast<PlayerId>(m_players.size())); // they will get next id available, client-host having id 0
    return m_players[id];
}

void GameConfig::addPlayer(const std::string &name) {
    m_players.push_back(name);
}

void GameConfig::removePlayer(std::string name) {
    for (auto it =m_players.begin(); it !=m_players.end(); ++it) {
        if (*it == name) {
            m_players.erase(it);
            return;
        }
    }
}

void GameConfig::copySettingsFromConfig(const GameConfig &config) {
    m_numPlayers = config.m_numPlayers;
    m_winningVictoryPoints = config.m_winningVictoryPoints;
    m_boardType=config.m_boardType;
}
