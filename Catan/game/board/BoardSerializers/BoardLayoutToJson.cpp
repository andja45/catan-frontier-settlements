//
// Created by matija on 1/4/26.
//

#include "BoardLayoutToJson.hpp"

#include <fstream>
#include <iostream>
#include <json.hpp>
using json=nlohmann::json;

void BoardLayoutToJson::serialize(Board board) {
    nlohmann::json saveBoard = json::array();
    for (const auto& tile : board.getTiles()) {
        saveBoard.push_back({
            {"q", tile->getTileCoord().q()},
            {"r", tile->getTileCoord().r()},
            {"type", tile->getResourceType()},
            {"number", tile->getNumber()}
        });
    }

    json root;
    root = saveBoard;

    std::ofstream saveFile(m_filePath);
    if (!saveFile.is_open()) {
        std::cerr << "Cannot open file for writing: " << m_filePath << std::endl;
        return;
    }

    saveFile << root.dump(4);
    saveFile.close();
}
