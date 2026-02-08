#ifndef GAMEDATA_H
#define GAMEDATA_H


#include <vector>
#include <map>
#include <string>
#include<ctime>
#include "../types/ResourceType.h"
#include<json.hpp>
#include<iostream>

class GameData {
private:
    // INFO:
    std::string m_gameName="";
    std::string m_datetime;

    // STATE:
    bool m_isGameWon = false;
    int m_numOfTurns = 0;

    // PLAYERS:
    std::vector<std::string> m_playerNames;
    std::map<std::string, int> m_pointsByPlayer;

    // AWARDS:
    std::string m_winningPlayer;
    std::string m_longestRoadOwner;
    std::string m_largestArmyOwner;

    // STATISTICS:
    std::map<int, int> m_diceRolls;
    std::map<ResourceType, int> m_resourceRolls;

public:
    GameData(){};
    GameData(std::string gameId, std::vector<std::string> playerNames);

    void initialize();
    void markGameWon() {m_isGameWon = true;}

    int getTurns() const {return m_numOfTurns;}
    void addTurn() {m_numOfTurns++;}

    void addDiceRoll(int diceResult) {m_diceRolls[diceResult]++;}
    void addResourceRoll(ResourceType resourceResult, int amount) {m_resourceRolls[resourceResult] += amount;}

    void setPlayerPoints(const std::string& playerName, int points) {m_pointsByPlayer[playerName] = points;}
    void setWinningPlayer(std::string winningPlayer) {m_winningPlayer = winningPlayer;}
    void setLargestArmyOwner(std::string largestArmyOwner) {m_largestArmyOwner = largestArmyOwner;}
    void setLongestRoadOwner(std::string longestRoadOwner) {m_longestRoadOwner = longestRoadOwner;}

    std::string getGameName() const {return m_gameName;}
    int getNumOfPlayers() const {return static_cast<int>(m_playerNames.size());}
    bool isGameWon() const {return m_isGameWon;}

    std::vector<std::string> getPlayerNames() const {return m_playerNames;}
    std::map<std::string,int> getPointsByPlayer() const {return m_pointsByPlayer;}
    std::map<int,int> getDiceRolls() const {return m_diceRolls;}
    std::map<ResourceType,int> getResourceRolls() const {return m_resourceRolls;}
    std::string getDate() const;

    std::string getWinningPlayer() const {return m_winningPlayer;}
    std::string getLongestRoadOwner() const {return m_longestRoadOwner;}
    std::string getLargestArmyOwner() const {return m_largestArmyOwner;}

    void loadFromJson(const nlohmann::json &jsonData);

    void writeToFile(const std::string &path) const;

    nlohmann::json toJson() const;

    static std::string getHistoryPath();
};
#endif // GAMEDATA_H
