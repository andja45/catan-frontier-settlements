//
// Created by andja on 10.12.25.
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
#include <unordered_set>
#include <vector>
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/NodeCoords.hpp>
#include <board/Coords/EdgeCoords.hpp>

struct TileDef;


void Board::clearBoard() {
    m_tiles.clear();
    m_nodes.clear();
    m_edges.clear();
    m_tilesByCoord.clear();
    m_tilesByNumber.clear();
}

// initialize tiles edge nodes and connects them together
void Board::initializeBoard(std::vector<TileDef> tileMap) { //TODO ROBBER AND PORTS
    clearBoard();

    for (const auto&[q, r, res, number] : tileMap) {
        auto t = std::make_unique<Tile>(q, r, res, number);

        Tile* raw = t.get();
        m_tiles.push_back(std::move(t));
        AxialCoords ax(q,r);
        m_tilesByCoord[ax]=raw;

        if (number >= 2 && number <= 12)
            m_tilesByNumber[number].push_back(raw);
    }

    connectBoardElements();
}
void Board::connectBoardElements(){

    for ( auto&[coord, ptr] : m_tilesByCoord) {
        Tile* t = ptr;
        for (auto nodeCoord : t->getTileCoord().getNodeCoords()){
            auto it = m_nodesByCoord.find(nodeCoord);
            Node* raw=nullptr;
            if (it==m_nodesByCoord.end()) {
                auto n = std::make_unique<Node>(nodeCoord);
                m_nodes.push_back(std::move(n));
                raw=n.get();
            }
            else raw=it->second;

            t->addAdjacentNode(raw);
            raw->addAdjacentTile(t);
        }
        for (auto edgeCoord : t->getTileCoord().getEdgeCoords()){
            NodeCoords node1=edgeCoord.getNodeCoordBefore();
            NodeCoords node2=edgeCoord.getNodeCoordAfter();


        }
    }
}


std::vector<Tile *> Board::getTilesWithNumber(int num) {
}

Tile * Board::getTileAt(TileCoords coords) {
}

Node * Board::getNodeAt(NodeCoords) {
}

Edge * Board::getEdgeAt(EdgeCoords) {
}

Node * Board::getNodeAt(TileCoords coords, NodeIndex index) {
}

Edge * Board::getEdgeAt(TileCoords coords, EdgeIndex index) {
}

Node * Board::getNodeById(NodeId nodeId) const {
}

Edge * Board::getEdgeById(EdgeId edgeId) const {
}

Tile * Board::getTileById(TileId tileId) const {
}

Node * Board::getNodeAtDir(TileCoords coords, PointDirection) {
}

Edge * Board::getEdgeAtDir(TileCoords coords, SideDirection) {
}

Tile * Board::getTileAtDir(TileCoords coords, SideDirection) {
}

std::vector<TileCoords> Board::getBoardCords() {
    std::vector<TileCoords> coords;
    for (auto&[coord, tile] : m_tilesByCoord) {
        coords.push_back(coord);
    }
    return coords;
}
