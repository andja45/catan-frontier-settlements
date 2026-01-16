//
// Created by andja on 9.12.25..
//

#ifndef Catan_EDGE_H
#define Catan_EDGE_H

#include <stdexcept>

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

    EdgeCoords m_edgeCoords;


public:
    Edge(EdgeId id,EdgeCoords ec) : m_edgeId(id), m_edgeCoords(ec) {}
    Edge(EdgeId id,EdgeCoords ec, Node *start, Node *end) : m_start(start), m_end(end), m_edgeId(id), m_edgeCoords(ec) {}


    Node* getStart() const { return m_start; }
    Node* getEnd() const { return m_end; }
    std::vector<Node*> getNodes() const { return {m_start,m_end}; }
    void setNodes(Node* start, Node* end) {
        m_start=start;
        m_end=end;
        if (m_start==nullptr || m_end==nullptr)
            throw std::invalid_argument("Edge must have both start and end node set");
    }
    void setId(EdgeId id) {m_edgeId=id;}

    bool isOccupied() const { return m_isOccupied; }
    int getOwner() const { return m_playerId; }
    int getEdgeId() const { return m_edgeId; }
    EdgeCoords getEdgeCoords() const { return m_edgeCoords; }

    void setOwner(int playerId) {m_playerId=playerId; m_isOccupied=true;}

    friend bool operator==(const Edge &lhs, const Edge &rhs) {
        return lhs.m_edgeId == rhs.m_edgeId;
    }
    friend bool operator!=(const Edge &lhs, const Edge &rhs) {
        return !(lhs == rhs);
    }

    bool hasTrade() const;
    TradeType getTradeType();
    EdgeDirection getDirection();

};

#endif //Catan_EDGE_H
