//
// Created by andja on 9.12.25..
//

#ifndef Catan_EDGE_H
#define Catan_EDGE_H

#include "Node.h"
#include "types/TypeAliases.h"
#include <board/Coords/EdgeCoords.hpp>
class Edge {
private:
    Node* m_start =nullptr;
    Node* m_end   =nullptr;

    bool m_isOccupied  =false; //is road built
    PlayerId m_playerId =-1;

    EdgeId m_edgeId=-1;
    EdgeCoords m_edgeCoord;

    inline static int m_numOfEdges=0;

public:
    Edge(EdgeCoords e, Node *start, Node *end) : Edge(e.q(), e.r(), e.i(), start, end) {}
    Edge(int q, int r, int i, Node *start, Node *end) : m_start(start), m_end(end) {
        m_numOfEdges++; m_edgeId =m_numOfEdges;
        m_edgeCoord={q,r,i};
    }

    Node* getStart() const { return m_start; }
    Node* getEnd() const { return m_end; }
    std::array<Node*,2> getNodes() const { return {m_start,m_end}; }


    bool isOccupied() const { return m_isOccupied; }
    int getOwner() const { return m_playerId; }
    int getEdgeId() const { return m_edgeId; }
    EdgeCoords getTileCoord() const { return m_edgeCoord; }

    void setNodes(Node* node1, Node* node2) {m_start=node1;m_end=node2;}
    void occupy(int playerId) {m_isOccupied=true; m_playerId=playerId;}

    friend bool operator==(const Edge &lhs, const Edge &rhs) {
        return lhs.m_edgeId == rhs.m_edgeId;
    }
    friend bool operator!=(const Edge &lhs, const Edge &rhs) {
        return !(lhs == rhs);
    }
};

#endif //Catan_EDGE_H
