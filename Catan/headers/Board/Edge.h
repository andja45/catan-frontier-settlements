//
// Created by andja on 9.12.25..
//

#ifndef Catan_EDGE_H
#define Catan_EDGE_H

#include "Node.h"
#include "../Types/TypeAliases.hpp"

class Edge {
private:
    Node* m_start=nullptr;
    Node* m_end=nullptr;

    bool m_isRoad=false;
    int m_playerId=-1;

    int m_edgeId=-1;
    HexCoords m_tileCoord{-1,-1};
    int m_edgeIndex=-1;
    static int m_numOfEdges;

public:
    Edge() {m_numOfEdges++; m_edgeId=m_numOfEdges;}
    Edge(int q, int r, int i, Node *start, Node *end) : m_start(start), m_end(end) {
        m_numOfEdges++; m_edgeId=m_numOfEdges;
        m_tileCoord={q,r}; m_edgeIndex=i;
    }

    Node* getStart() const { return m_start; }
    Node* getEnd() const { return m_end; }
    bool isRoad() const { return m_isRoad; }
    int getPlayerId() const { return m_playerId; }
    int getEdgeId() const { return m_edgeId; }
    HexCoords getTileCoord() const { return m_tileCoord; }
    int getEdgeIndex() const { return m_edgeIndex; }

    void setNodes(Node* node1, Node* node2) {m_start=node1;m_end=node2;}
    void setRoad(int playerId) {m_isRoad=true; m_playerId=playerId;}
};


#endif //Catan_EDGE_H
