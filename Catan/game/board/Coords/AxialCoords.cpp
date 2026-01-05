//
// Created by matija on 1/4/26.
//

#include "./AxialCoords.hpp"
#include "EdgeCoords.hpp"
#include "NodeCoords.hpp"
#include <functional>


AxialCoords AxialCoords::operator+(const AxialCoords &other) const {
    return AxialCoords(m_q + other.m_q, m_r + other.m_r);
}

AxialCoords AxialCoords::getNeighborCoords(TileDirection dir) {
    static const std::array<AxialCoords,6> directionCoords={{
        { 0, -1 },  // top left
        { 1, -1 },  // top right
        { 1,  0 },  // right
        { 0,  1 },  // bottom right
        { -1, 1 },  // bottom left
        { -1, 0 }   // left
    }};

    return directionCoords[static_cast<int>(dir)];
}

EdgeCoords AxialCoords::getEdgeCoordsAt(EdgeDirection dir) const {
    return EdgeCoords(*this,dir);
}

NodeCoords AxialCoords::getNodeCoordsAt(NodeDirection dir) const {
    return NodeCoords(*this,dir);
}

std::vector<EdgeCoords> AxialCoords::getEdgeCoords() {
    std::vector<EdgeCoords> coords;
    for (int i = 0; i < static_cast<int>(EdgeDirection::End); i++) {
        coords.push_back(getEdgeCoordsAt(static_cast<EdgeDirection>(i)));
    }
    return coords;
}

std::vector<NodeCoords> AxialCoords::getNodeCoords() {
    std::vector<NodeCoords> coords;
    for (int i = 0; i < static_cast<int>(NodeDirection::End); i++) {
        coords.push_back(getNodeCoordsAt(static_cast<NodeDirection>(i)));
    }
    return coords;
}
