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


public:
    Edge(EdgeId id) : m_edgeId(id) {}
    Edge(EdgeId id, Node *start, Node *end) : m_start(start), m_end(end), m_edgeId(id) {}

    Node* getStart() const { return m_start; }
    Node* getEnd() const { return m_end; }
    std::vector<Node*> getNodes() const { return {m_start,m_end}; }
    void setNodes(Node* start, Node* end) { m_start=start; m_end=end; }
    void setId(EdgeId id) {m_edgeId=id;}

    bool isOccupied() const { return m_isOccupied; }
    int getOwner() const { return m_playerId; }
    int getEdgeId() const { return m_edgeId; }

    void setPlayer(int playerId) {m_playerId=playerId;}

    friend bool operator==(const Edge &lhs, const Edge &rhs) {
        return lhs.m_edgeId == rhs.m_edgeId;
    }
    friend bool operator!=(const Edge &lhs, const Edge &rhs) {
        return !(lhs == rhs);
    }
};

#endif //Catan_EDGE_H
