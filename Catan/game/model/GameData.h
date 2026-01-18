#ifndef GAMEDATA_H
#define GAMEDATA_H


#include <vector>
#include <map>
#include <string>
#include<ctime>
#include "../types/ResourceType.h"
#include<json.hpp>
#include<iostream>

class GameData
{
    private:
    int m_gameId = -1;
    int m_numOfPlayers = 0;
    int m_numOfTurns = 0;
    std::string m_datetime;
    bool m_isGameWon = false;
    std::vector<std::string> m_playerNames;
    std::string m_winningPlayer;
    std::string m_longestRoadOwner;
    std::string m_biggestArmyOwner;
    std::map<std::string,int> m_pointsByPlayer;
    std::map<int,int> m_diceRolls;
    std::map<ResourceType,int> m_resourceRolls;

public:
    GameData(){};
    GameData(int gameId, std::vector<std::string> playerNames);

    void inicializeGameData();
    void loadFromJson(const nlohmann::json &jsonData);
    nlohmann::json toJson();

    void setGameWon() {m_isGameWon = true;}
    int getNumOfPlayers() const {return m_numOfPlayers;}
    void addDiceRoll(int diceResult) {m_diceRolls[diceResult]++;}
    void addResourceRoll(ResourceType resourceResult) {m_resourceRolls[resourceResult]++;}
    void setWinningPlayer(std::string winningPlayer) {m_winningPlayer = winningPlayer;}
    void setBiggestArmyOwner(std::string biggestArmyOwner) {m_biggestArmyOwner = biggestArmyOwner;}
    void setLongestRoadOwner(std::string longestRoadOwner) {m_longestRoadOwner = longestRoadOwner;}

    int getTurns() const {return m_numOfTurns;}
    void addTurn() {m_numOfTurns++;}

    std::map<std::string,int> getPointsByPlayer() const {return m_pointsByPlayer;}
    std::map<int,int> getDiceRolls() const {return m_diceRolls;}
    std::map<ResourceType,int> getResourceRolls() const {return m_resourceRolls;}

    std::string getLongestRoadOwner() const {return m_longestRoadOwner;}
    std::string getBiggestArmyOwner() const {return m_biggestArmyOwner;}

    std::string getWinningPlayer() const {return m_winningPlayer;}

    std::vector<std::string> getPlayerNames() const {return m_playerNames;}
    int getId() const {return m_gameId;}
    bool isGameWon() const {return m_isGameWon;}
    std::string getDate() const;
};
#endif // GAMEDATA_H
