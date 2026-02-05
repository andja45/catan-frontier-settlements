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
private:
    std::string m_gameName;
    int m_numPlayers = 0;
    int m_winningVictoryPoints = 10;
    BoardType m_boardType= BoardType::Classic;
    std::vector<std::string> m_players;
public:
    GameConfig() = default;

    GameConfig(int numPlayers,
               int winningPoints,
               std::vector<std::string> players);

    const std::string& player(PlayerId id) const;
    void addPlayer(const std::string &name);
    void removePlayer(std::string name);
    void copySettingsFromConfig(const GameConfig& config);
    int getMaxPlayers() const {return m_numPlayers;}
    std::vector<std::string> getPlayerNames() const {return m_players;}
    int getPointsToWin() const {return m_winningVictoryPoints;}
    BoardType getBoardType() const {return m_boardType;}
    void setBoardType(BoardType boardType) {this->m_boardType=boardType;}
    void setNumPlayers(int numPlayers) {this->m_numPlayers=numPlayers;}
    void setWinningPoints(int winningPoints) {this->m_winningVictoryPoints=winningPoints;}
    void setPlayers(std::vector<std::string> players) {this->m_players=std::move(players);}
    void setName(const std::string &name)  {m_gameName=name;}
    std::string getName() const {return m_gameName;}
};

#endif //CATAN_GAMECONFIG_H