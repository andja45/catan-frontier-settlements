//
// Created by matija on 1/4/26.
//

#ifndef CATAN_NODECOORDS_HPP
#define CATAN_NODECOORDS_HPP
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/ElementCoords.hpp>

class NodeCoords:public ElementCoords {
public:
    using ElementCoords::ElementCoords;
    NodeCoords(AxialCoords ax, NodeDirection dir);
    NodeCoords(int q, int r, int i) : NodeCoords({q,r},static_cast<NodeDirection>(i)) {}
    NodeDirection direction() const { return static_cast<NodeDirection>(m_i); }
};


namespace std {
    template <>
    struct hash<NodeCoords> {
        std::size_t operator()(const NodeCoords& a) const noexcept {
            return a.elementHash();
        }
    };
}

#endif //CATAN_NODECOORDS_HPP