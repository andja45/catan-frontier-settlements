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

    for (ResourceType resource : resourceCardTypes) {
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

    nlohmann::json diceJson = nlohmann::json::object();
    for (const auto& [dice, count] : m_diceRolls) {
        diceJson[std::to_string(dice)] = count;
    }
    jsonData["diceRolls"] = diceJson;
    nlohmann::json resourceRollsJson = nlohmann::json::object();
    for (const auto &[resource, count] : m_resourceRolls) {
        resourceRollsJson[toString(resource)] = count;
    }
    jsonData["resourceRolls"] = resourceRollsJson;

    return jsonData;
}

std::string GameData::getHistoryPath() {
    return "history.json";
}

void GameData::loadFromJson(const nlohmann::json &jsonData) {
    m_gameName       = jsonData.at("gameId").get<std::string>();
    m_numOfTurns = jsonData.at("numOfTurns").get<int>();
    m_datetime     = jsonData.at("datetime").get<std::string>();
    m_isGameWon    = jsonData.at("isGameWon").get<bool>();
    m_playerNames  = jsonData.at("playerNames").get<std::vector<std::string>>();
    m_winningPlayer = jsonData.at("winningPlayer").get<std::string>();
    m_largestArmyOwner = jsonData.at("largestArmyOwner").get<std::string>();
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

void GameData::writeToFile(const std::string& path) const
{
    nlohmann::json history = nlohmann::json::array();
    std::string historyPath = path+getHistoryPath();

    std::ifstream in(historyPath);
    if (in.good()) {
        try {
            in >> history;
            if (!history.is_array()) {
                std::cerr << ("History file is not a JSON array");
            }
        } catch (...) {
            std::cerr<<("Failed to parse history.json");
        }
    }


    history.push_back(toJson());

    std::ofstream out(historyPath, std::ios::trunc);
    if (!out) {
        std::cerr << "Failed to open history file: "
               << std::strerror(errno) << "\n";
    }

    out << history.dump(4);
}
