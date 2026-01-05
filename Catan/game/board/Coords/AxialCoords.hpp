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
    static AxialCoords getNeighborCoords(TileDirection dir);

    EdgeCoords getEdgeCoordsAt(EdgeDirection dir) const;
    NodeCoords getNodeCoordsAt(NodeDirection dir) const;

    std::vector<EdgeCoords> getEdgeCoords();
    std::vector<NodeCoords> getNodeCoords();

    friend bool operator==(const AxialCoords &lhs, const AxialCoords &rhs) {
        return lhs.m_q == rhs.m_q
               && lhs.m_r == rhs.m_r;
    }
    friend bool operator!=(const AxialCoords &lhs, const AxialCoords &rhs) {
        return !(lhs == rhs);
    }
};


namespace std {
    template <>
    struct hash<AxialCoords> {
        std::size_t operator()(const AxialCoords& a) const noexcept {
            std::size_t h1 = std::hash<int>{}(a.q());
            std::size_t h2 = std::hash<int>{}(a.r());
            return h1 ^ (h2 << 1);
        }
    };
}

#endif //CATAN_AXIALCOORDS_HPP