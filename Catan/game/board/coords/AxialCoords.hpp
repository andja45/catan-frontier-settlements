//
// Created by matija on 1/4/26.
//

#ifndef CATAN_AXIALCOORDS_HPP
#define CATAN_AXIALCOORDS_HPP

#include <types/Directions.h>
#include <types/TypeAliases.h>
#include <vector>

class EdgeCoords;
class NodeCoords;


// axial coordinate system for tile indexing in hex grid
// checkout https://www.redblobgames.com/grids/hexagons/ for more info
class AxialCoords {
private:
    int m_q;
    int m_r;
public:
    AxialCoords() = default;
    AxialCoords(int q, int r) : m_q(q), m_r(r) {}
    int q() const { return m_q; }
    int r() const { return m_r; }

    AxialCoords operator+(const AxialCoords &other) const;
    AxialCoords getNeighborCoords(TileDirection dir); // get surrounding tile coords

    // edge and node coords are made of axial plus direction
    EdgeCoords getEdgeCoordsAt(EdgeDirection dir) const;
    NodeCoords getNodeCoordsAt(NodeDirection dir) const;

    // get all edges and nodes around hex
    std::vector<EdgeCoords> getEdgeCoords() const;
    std::vector<NodeCoords> getNodeCoords() const;

    friend bool operator==(const AxialCoords &lhs, const AxialCoords &rhs) {
        return lhs.m_q == rhs.m_q
               && lhs.m_r == rhs.m_r;
    }
    friend bool operator!=(const AxialCoords &lhs, const AxialCoords &rhs) {
        return !(lhs == rhs);
    }
};


template <>
struct std::hash<AxialCoords> {
    std::size_t operator()(const AxialCoords& a) const noexcept {
        std::size_t h1 = std::hash<int>{}(a.q());
        std::size_t h2 = std::hash<int>{}(a.r());
        return h1 ^ (h2 << 1);
    }
};

#endif //CATAN_AXIALCOORDS_HPP