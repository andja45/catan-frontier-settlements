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
    std::string m_datetime;
    bool m_isGameWon = false;
    std::vector<std::string> m_playerNames;
    std::string m_winningPlayer;
    std::map<std::string,int> m_pointsByPlayer;
    std::map<int,int> m_diceRolls;
    std::map<ResourceType,int> m_resourceRolls;

public:
    GameData(){};
    GameData(int gameId) : m_gameId(gameId){};


    void inicializeGameData();
    void loadFromJson(const nlohmann::json &j);
    nlohmann::json toJson();

    int getNumOfPlayers() const {return m_numOfPlayers;}
    void addDiceRoll(int diceResult) {m_diceRolls[diceResult]++;}
    void addResourceRoll(ResourceType resourceResult) {m_resourceRolls[resourceResult]++;}


    std::map<std::string,int> getPointsByPlayer() const {return m_pointsByPlayer;}
    std::map<int,int> getDiceRolls() const {return m_diceRolls;}
    std::map<ResourceType,int> getResourceRolls() const {return m_resourceRolls;}

    std::string  getPlayerNamesString() const;
    std::string getWinningPlayer() const {return m_winningPlayer;}

    std::vector<std::string> getPlayerNames() const {return m_playerNames;}
    int getId() const {return m_gameId;}
    void setGameWon() {m_isGameWon = true;}
    bool isGameWon() const {return m_isGameWon;}
    std::string getDate() const;
};
#endif // GAMEDATA_H
