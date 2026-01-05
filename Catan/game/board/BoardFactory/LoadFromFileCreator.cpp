//
// Created by matija on 1/5/26.
//

#include "LoadFromFileCreator.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

std::vector<TileDef> LoadFromFileCreator::loadBoardFromFile() {
    std::filesystem::path filePath(m_filePath);
    std::ifstream fileStream(filePath);
    std::string ext = filePath.extension().string();
    std::vector<TileDef> tiles;
    if (ext==".txt") {
        tiles=LoadFromFileCreator::loadBoardFromTxt(fileStream);
    }
    if (ext==".json"){
        tiles=LoadFromFileCreator::loadBoardFromJson(fileStream);
    }
    return tiles;
}

std::vector<TileDef> LoadFromFileCreator::loadBoardFromTxt(std::istream &loadFile) {
    std::vector<TileDef> loadMap;
    int q, r, number;
    std::string type;

    std::string line;
    while (std::getline(loadFile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        if (!(iss >> q >> r >> type >> number)) {
            std::cerr << "Error in line: " << line << std::endl;
            throw std::runtime_error("Failed to parse JSON file.");
            continue;
        }

        loadMap.push_back({q, r, fromString(type), number});
    }

    return loadMap;

}

std::vector<TileDef> LoadFromFileCreator::loadBoardFromJson(std::istream &loadFile) {
    json root;
    try {
        loadFile >> root;
    } catch (json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        throw std::runtime_error("Failed to parse JSON file.");
    }

    const json& savedBoard = root;
    std::vector<TileDef> loadMap;

    for (const auto& tile : savedBoard) {
        loadMap.push_back({
            tile.at("q").get<int>(),
            tile.at("r").get<int>(),
            fromString( tile.at("type").get<std::string>()),
            tile.at("number").get<int>()
        });
    }
    return loadMap;
}

std::unique_ptr<Board> LoadFromFileCreator::getBoard() {
    auto board=std::make_unique<Board>();
    auto tiles = loadBoardFromFile();
    board->initializeBoard(tiles);
    return std::move(board);
}
