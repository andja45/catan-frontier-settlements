//
// Created by matija on 1/4/26.
//

#include "BoardLayoutToJson.hpp"

#include <fstream>
#include <iostream>
#include <json.hpp>
using json=nlohmann::json;

void BoardLayoutToJson::serialize(const Board& board) {
    json root;
    root["type"] = "catan_board";

    root["tiles"] = json::array();
    for (const auto& tile : board.getTiles()) {
        root["tiles"].push_back({
            {"q", tile->getTileCoord().q()},
            {"r", tile->getTileCoord().r()},
            {"resource", toString(tile->getResourceType())},
            {"number", tile->getNumber()}
        });
    }

    root["ports"] = json::array();
    for (const auto& port : board.getPortDefs()) {
        root["ports"].push_back({
            {"q", port.q},
            {"r", port.r},
            {"dir", port.i},
            {"resource", toString(port.tradeType)}
        });
    }

    std::ofstream saveFile(m_filePath);
    if (!saveFile) {
        std::cerr << "Cannot open file for writing: " << m_filePath << std::endl;
        return;
    }

    saveFile << root.dump(4);
}