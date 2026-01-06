//
// Created by andja on 10.12.25.
//

#include "Board.h"
#include "Node.h"
#include <algorithm>
#include <fstream>
#include <memory>
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


// initializes tiles edge nodes and connects them together
void Board::initializeBoard(std::vector<TileDef> tileMap) { //TODO ROBBER AND PORTS, break into parts?
    clearBoard();

    for (const auto&[q, r, res, number] : tileMap) {
        // make tile for each tiledef
        AxialCoords ax(q,r);

        auto t = std::make_unique<Tile>(q, r, res, number);
        Tile* rawTile = t.get();
        m_tiles.push_back(std::move(t));
        m_tilesByCoord[ax]=rawTile;

        if (number >= 2 && number <= 12)
            m_tilesByNumber[number].push_back(rawTile);

        // make nodes
        auto nodeCoords = ax.getNodeCoords();
        auto edgeCoords = ax.getEdgeCoords();
        int numOfElements=nodeCoords.size();

        for (auto nc:nodeCoords) {
            int id=m_nodes.size();
            auto n = std::make_unique<Node>(id);
            Node* rawNode = n.get();
            m_nodesByCoord[nc]=rawNode;
        }

        // make edges
        for (auto ec:edgeCoords) {
            int id=m_edges.size();
            auto e = std::make_unique<Edge>(id);
            Edge* rawEdge = e.get();
            m_edgesByCoord[ec]=rawEdge;
        }

        // connect elements
        for (int i=0; i<numOfElements; i++) {
            NodeCoords nc=nodeCoords[i];
            NodeCoords nnc=nodeCoords[(i+1)%numOfElements];
            EdgeCoords ec=edgeCoords[i];

            Edge* edge=getEdgeAt(ec);
            Node* node=getNodeAt(nc);
            Node* nextNode=getNodeAt(nnc);

            edge->setNodes(node,nextNode);
            node->addAdjacentEdge(edge);
            nextNode->addAdjacentEdge(edge);

            node->addAdjacentTile(rawTile);
        }

    }

}

