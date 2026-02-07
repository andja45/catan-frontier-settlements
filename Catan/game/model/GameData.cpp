#include "GameData.h"

#include <fstream>
#include <time.h>

GameData::GameData(std::string gameId, std::vector<std::string> playerNames) : m_gameName(gameId), m_playerNames(playerNames) {
    initialize();
}

void GameData::initialize() {
    m_numOfTurns = 0;
    m_isGameWon = false;

    m_winningPlayer.clear();
    m_largestArmyOwner.clear();
    m_longestRoadOwner.clear();

    m_pointsByPlayer.clear();
    for (const auto& name : m_playerNames) {
        m_pointsByPlayer[name] = 0;
    }

    std::time_t now = std::time(NULL);
    std::tm tm{};

    #ifdef _WIN32
        // Windows uses localtime_s
        localtime_s(&tm, &now);
    #else
        // Linux/POSIX uses localtime_r
        localtime_r(&now, &tm);
    #endif

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

nlohmann::json GameData::toJson() const {
    nlohmann::json jsonData;

    jsonData["gameId"]        = m_gameName;
    jsonData["numOfTurns"]  = m_numOfTurns;
    jsonData["datetime"]      = m_datetime;
    jsonData["isGameWon"]     = m_isGameWon;
    jsonData["playerNames"]   = m_playerNames;
    jsonData["winningPlayer"] = m_winningPlayer;
    jsonData["largestArmyOwner"] = m_largestArmyOwner;
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

std::string GameData::getHistoryPath() {
    return "./resources/history.json";
}

void GameData::loadFromJson(const nlohmann::json &jsonData) {
    m_gameName       = jsonData.at("gameId").get<std::string>();
    m_numOfTurns = jsonData.at("numOfTurns").get<int>();
    m_datetime     = jsonData.at("datetime").get<std::string>();
    m_isGameWon    = jsonData.at("isGameWon").get<bool>();
    m_playerNames  = jsonData.at("playerNames").get<std::vector<std::string>>();
    m_winningPlayer = jsonData.at("winningPlayer").get<std::string>();
    m_largestArmyOwner = jsonData.at("biggestArmyOwner").get<std::string>();
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

void GameData::writeToFile() const
{
    const char* historyPath = GameData::getHistoryPath().data();

    std::ofstream out(historyPath, std::ios::app);
    if (!out) {
        throw std::runtime_error("Failed to open file");
    }

    out << toJson().dump(4)<<'\n';
}


