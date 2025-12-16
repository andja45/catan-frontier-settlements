//
// Created by andja on 9.12.25..
//

#ifndef Catan_EDGE_H
#define Catan_EDGE_H

#include "Node.h"
#include "../Types/TypeAliases.hpp"

class Edge {
private:
    Node* _start=nullptr;
    Node* _end=nullptr;

    bool _isRoad=false;
    int _playerId=-1;

    int _edgeId=-1;
    HexCoords _tileCoord{-1,-1};
    int edgeIndex=-1;
    static int _numOfEdges;

public:
    Edge() {_numOfEdges++; _edgeId=_numOfEdges;}
    Edge(int q, int r, int i, Node *start, Node *end) : _start(start), _end(end) {
        _numOfEdges++; _edgeId=_numOfEdges;
        _tileCoord={q,r}; edgeIndex=i;
    }

    Node* getStart() const { return _start; }
    Node* getEnd() const { return _end; }
    bool isRoad() const { return _isRoad; }
    int getPlayerId() const { return _playerId; }
    int getEdgeId() const { return _edgeId; }
    HexCoords getTileCoord() const { return _tileCoord; }
    int getEdgeIndex() const { return edgeIndex; }

    void setNodes(Node* node1, Node* node2) {_start=node1;_end=node2;}
    void setRoad(int playerId) {_isRoad=true; _playerId=playerId;}
};


#endif //Catan_EDGE_H