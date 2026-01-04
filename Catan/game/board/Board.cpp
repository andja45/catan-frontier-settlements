//
// Created by andja on 10.12.25..
//

#include "Board.h"
#include "Node.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <types/TypeAliases.h>
#include <algorithm>

#include <json.hpp>

struct TileDef;
using json = nlohmann::json;

// inits static fields, in separate files to avoid clutter
#include "BoardData.h"
void Board::standardizeEdgeCoords(HexCoords &coords, int &index) {
}

void Board::clearBoard() {
    m_tiles.clear();
    m_nodes.clear();
    m_edges.clear();
    m_tilesByCoord.clear();
    m_tilesByNumber.clear();
}

// initialize tiles edge nodes and connects them together
void Board::initializeBoard() {
    clearBoard();

    std::vector<TileDef> tileMap = generateRandomBoard();  // TODO why only random?

    for (const auto&[q, r, res, number] : tileMap) {
        auto t = std::make_unique<Tile>(q, r, res, number);

        Tile* raw = t.get();
        m_tiles.push_back(std::move(t));
        m_tilesByCoord[{q,r}]=raw;

        if (number >= 2 && number <= 12)
            m_tilesByNumber[number].push_back(raw);
    }

    connectBoardElements();
}
void Board::connectBoardElements(){
    for ( auto&[coord, ptr] : m_tilesByCoord) {
        Tile* t = ptr;

        for (int i = static_cast<int>(PointDirection::Top); i < static_cast<int>(PointDirection::End); ++i) {
            PointDirection dir=static_cast<PointDirection>(i);
            HexCoords _coord=coord;
            NodeIndex _index=i;
            Board::standardizeNodeCoords(_coord,_index);
            Node *raw = m_tilesByCoord[_coord]->getNodeAt(i);
            if (raw==nullptr) {
                auto n = std::make_unique<Node>(_coord.first, _coord.second, i);
                m_nodes.push_back(std::move(n));
                raw=n.get();
            }
            t->setAdjacentNode(raw,i);
            raw->addAdjacentTile(t);
        }

        for (int i = 0,j=0; i < static_cast<int>(PointDirection::End) && j< static_cast<int>(SideDirection::End); ++i,++j) {
            int i_next=(i+1)%static_cast<int>(PointDirection::End);
            SideDirection edgeDir=static_cast<SideDirection>(j);

            int _edgeIndex=j;
            HexCoords _coord=coord;
            Board::standardizeEdgeCoords(_coord,_edgeIndex);
            Edge *raw = m_tilesByCoord[_coord]->getEdgeAt(_edgeIndex);
            if (raw==nullptr) {
                auto e = std::make_unique<Edge>(_coord.first, _coord.second, _edgeIndex, t->getNodeAt(i), t->getNodeAt(i_next));
                m_edges.push_back(std::move(e));
                raw=e.get();
            }
            t->setAdjacentEdge(raw,_edgeIndex);
            t->getNodeAt(i)->addAdjacentEdge(raw);
            t->getNodeAt(i_next)->addAdjacentEdge(raw);
        }

    }
}

// generates standard catan map with random resource distribution
std::vector<TileDef> Board::generateRandomBoard(){
    std::vector<TileDef> r;
    std::vector<ResourceType> hexList = {ResourceType::Desert};
    for(int i = 0; i < 4; i++) {
        hexList.push_back(ResourceType::Wood);
        hexList.push_back(ResourceType::Wheat);
        hexList.push_back(ResourceType::Wool);
    }
    for(int i = 0; i < 3; i++) {
        hexList.push_back(ResourceType::Ore);
        hexList.push_back(ResourceType::Brick);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hexList.begin(), hexList.end(), g);

    std::reverse(m_standardNumberOrder.begin(), m_standardNumberOrder.end());
    auto hexCoordinates = Board::m_standardCoordinates;
    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) r.push_back({hexCoordinates[i].first,hexCoordinates[i].second, hexList[i], 7});
        else r.push_back({hexCoordinates[i].first,hexCoordinates[i].second, hexList[i], m_standardNumberOrder[j++]});
    }
    return r;
}

void Board::saveBoard(const std::string& saveFilePath) {
    json saveBoard = json::array();

    for (auto [coords, t] : m_tilesByCoord) {
        saveBoard.push_back({
            {"q", coords.first},
            {"r", coords.second},
            {"type", t->getType()},
            {"number", t->getNumber()}
        });
    }

    json root;
    root = saveBoard;

    std::ofstream saveFile(saveFilePath);
    if (!saveFile.is_open()) {
        std::cerr << "Cannot open file for writing: " << saveFilePath << std::endl;
        return;
    }

    saveFile << root.dump(4);
    saveFile.close();
}


std::vector<TileDef> Board::loadSavedBoard(const std::string& loadFilePath) {

    std::ifstream loadFile(loadFilePath);
    if (!loadFile.is_open()) {
        std::cerr << "Cannot open file for reading: " << loadFilePath << std::endl;
        return {};
    }

    json root;
    try {
        loadFile >> root;
    } catch (json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return {};
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

std::vector<TileDef> Board::loadBoardFromTextFile(const std::string& loadFilePath) {
    clearBoard();
    std::ifstream loadFile(loadFilePath);
    if (!loadFile.is_open()) {
        std::cerr << "Cannot open file: " << loadFilePath << std::endl;
        return {};
    }

    std::vector<TileDef> loadMap;
    int q, r, number;
    std::string type;

    std::string line;
    while (std::getline(loadFile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        if (!(iss >> q >> r >> type >> number)) {
            std::cerr << "Error in line: " << line << std::endl;
            continue;
        }

        loadMap.push_back({q, r, fromString(type), number});
    }

    return loadMap;
}

std::vector<Tile *> Board::getTilesWithNumber(int num) {
}

Tile * Board::getTileAt(HexCoords coords) {
}

Node * Board::getNodeAt(HexCoords coords, NodeIndex index) {
}

Edge * Board::getEdgeAt(HexCoords coords, EdgeIndex index) {
}

Node * Board::getNodeById(NodeId nodeId) const {
}

Edge * Board::getEdgeById(EdgeId edgeId) const {
}

Tile * Board::getTileById(TileId tileId) const {
}

Node * Board::getNodeAtDir(HexCoords coords, PointDirection) {
}

Edge * Board::getEdgeAtDir(HexCoords coords, SideDirection) {
}

Tile * Board::getTileAtDir(HexCoords coords, SideDirection) {
}

std::vector<HexCoords> Board::getBoardCords() {
    std::vector<HexCoords> coords;
    for (auto&[coord, tile] : m_tilesByCoord) {
        coords.push_back(coord);
    }
    return coords;
}
