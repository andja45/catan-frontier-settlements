#include "GameData.h"
#include <time.h>

GameData::GameData(int gameId, std::vector<std::string> playerNames) : m_gameId(gameId), m_playerNames(playerNames) {
    inicializeGameData();
}

void GameData::inicializeGameData() {
    std::time_t now = std::time(NULL);
    std::tm tm{};
    localtime_s(&tm, &now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    m_datetime= oss.str();

    for (ResourceType resource : ResourceCardTypes) {
        m_resourceRolls[resource]=0;
    }
    for (int i = 2; i<=12; i++) {
        m_diceRolls[i]=0;
    }
}

std::string GameData::getDate() const {
    return m_datetime;
}

nlohmann::json GameData::toJson() {
    nlohmann::json jsonData;

    jsonData["gameId"]        = m_gameId;
    jsonData["numOfPlayers"]  = m_numOfPlayers;
    jsonData["numOfTurns"]  = m_numOfTurns;
    jsonData["datetime"]      = m_datetime;
    jsonData["isGameWon"]     = m_isGameWon;
    jsonData["playerNames"]   = m_playerNames;
    jsonData["winningPlayer"] = m_winningPlayer;
    jsonData["biggestArmyOwner"] = m_biggestArmyOwner;
    jsonData["longestRoadOwner"] = m_longestRoadOwner;
    jsonData["pointsByPlayer"] = m_pointsByPlayer;
    jsonData["diceRolls"]      = m_diceRolls;

    nlohmann::json resourceRollsJson = nlohmann::json::object();
    for (const auto &[resource, count] : m_resourceRolls) {
        resourceRollsJson[toString(resource)] = count;
    }
    jsonData["resourceRolls"] = resourceRollsJson;

    return jsonData;
}
void GameData::loadFromJson(const nlohmann::json &jsonData)
{

    m_gameId       = jsonData.at("gameId").get<int>();
    m_numOfPlayers = jsonData.at("numOfPlayers").get<int>();
    m_numOfTurns = jsonData.at("numOfTurns").get<int>();
    m_datetime     = jsonData.at("datetime").get<std::string>();
    m_isGameWon    = jsonData.at("isGameWon").get<bool>();
    m_playerNames  = jsonData.at("playerNames").get<std::vector<std::string>>();
    m_winningPlayer = jsonData.at("winningPlayer").get<std::string>();
    m_biggestArmyOwner = jsonData.at("biggestArmyOwner").get<std::string>();
    m_longestRoadOwner = jsonData.at("longestRoadOwner").get<std::string>();
    m_pointsByPlayer = jsonData.at("pointsByPlayer").get<std::map<std::string, int>>();

    m_diceRolls.clear();
    for (const auto &[dice, count] : jsonData.at("diceRolls").items()) {
        int diceValue = std::stoi(dice);
        m_diceRolls[diceValue] = count.get<int>();
    }
    m_resourceRolls.clear();
    for (const auto &[resource, count] : jsonData.at("resourceRolls").items()) {
        ResourceType type;
        if (!resourceFromString(resource, type)) {
            std::cerr << "Unknown resource type: " << resource << "\n";
            continue;
        }
        m_resourceRolls[type] = count.get<int>();
    }
}
