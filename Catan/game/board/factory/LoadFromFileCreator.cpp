//
// Created by matija on 1/5/26.
//

#include "LoadFromFileCreator.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

std::pair<std::vector<TileDef>, std::vector<PortDef>> LoadFromFileCreator::loadBoardFromFile() {
    std::filesystem::path filePath(m_filePath);
    std::pair<std::vector<TileDef>,std::vector<PortDef>> map;

    std::ifstream fileStream(filePath);

    if (!fileStream) {
        std::cerr<<"Failed to open file: " + filePath.string();
    }
    else {
        std::string ext = filePath.extension().string();
        if (ext==".txt") {
            map=LoadFromFileCreator::loadBoardFromTxt(fileStream);
        }
        else if (ext == ".json") {
            map = loadBoardFromJson(fileStream);
        }
        else {
            std::cerr<<"Unsupported file extension: " + ext;
        }
    }

    return map;
}

std::pair<std::vector<TileDef>, std::vector<PortDef>> LoadFromFileCreator::loadBoardFromTxt(std::istream &loadFile) {
    std::vector<TileDef> loadMap;
    std::vector<PortDef> loadPorts;
    int q, r, number, dir;
    std::string resource;
    std::string type;

    std::string line;
    while (std::getline(loadFile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);

        if (!(iss >> type)) {
            std::cerr << "Error in line: " << line << std::endl;
            continue;
        }
        if (type=="tile:") {
            if (!(iss >> q >> r >> resource >> number)) {
                std::cerr << "Error in line: " << line << std::endl;
                continue;
            }
            loadMap.push_back({q, r, fromString(resource), number});
        }
        else if (type=="port:") {
            if (!(iss >> q >> r >> dir >> resource)) {
                std::cerr << "Error in line: " << line << std::endl;
                continue;
            }
            loadPorts.push_back(PortDef{q, r, dir,fromString(resource)});
        }
        else {
            std::cerr << "Error in line: " << line << std::endl;
            continue;
        }

    }
    return {loadMap,loadPorts};
}

std::pair<std::vector<TileDef>, std::vector<PortDef>> LoadFromFileCreator::loadBoardFromJson(std::istream &loadFile)
{
    json root;
    try {
        loadFile >> root;
    } catch (json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        throw std::runtime_error("Failed to parse JSON file.");
    }

    if (!root.contains("type") || root.at("type") != "catan_board") {
        throw std::runtime_error("Invalid board JSON type");
    }

    std::vector<TileDef> loadMap;
    std::vector<PortDef> loadPorts;

    if (root.contains("tiles")) {
        for (const auto& tile : root.at("tiles")) {
            loadMap.push_back({
                tile.at("q").get<int>(),
                tile.at("r").get<int>(),
                fromString(tile.at("resource").get<std::string>()),
                tile.at("number").get<int>()
            });
        }
    }

    if (root.contains("ports")) {
        for (const auto& port : root.at("ports")) {
            loadPorts.push_back({
                port.at("q").get<int>(),
                port.at("r").get<int>(),
                port.at("dir").get<int>(),
                fromString(port.at("resource").get<std::string>())
            });
        }
    }

    return { loadMap, loadPorts };
}

std::unique_ptr<Board> LoadFromFileCreator::getBoard() {
    auto board=std::make_unique<Board>();
    auto [tiles,ports] = loadBoardFromFile();
    board->initializeBoard(tiles);
    board->loadPorts(ports);
    return std::move(board);
}
