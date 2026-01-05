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
    NodeCoords(AxialCoords ax, NodeDirection dir) : ElementCoords(ax.q(),ax.r(), static_cast<int>(dir)) {}
    NodeCoords standardize() const;
    NodeCoords next() const;
    NodeDirection direction() const {return static_cast<NodeDirection>(m_i);}

    friend bool operator==(const NodeCoords &lhs, const NodeCoords &rhs) {
        return static_cast<const ElementCoords &>(lhs) == static_cast<const ElementCoords &>(rhs);
    }
    friend bool operator!=(const NodeCoords &lhs, const NodeCoords &rhs) {
        return !(lhs == rhs);
    }
};


namespace std {
    template <>
    struct hash<NodeCoords> {
        std::size_t operator()(const NodeCoords& a) const noexcept {
            auto aa=a.standardize();
            std::size_t h1 = std::hash<int>{}(aa.getAxialCoords().q());
            std::size_t h2 = std::hash<int>{}(aa.getAxialCoords().r());
            std::size_t h3 = std::hash<int>{}(static_cast<int>(aa.direction()));
            return h1 ^ (h2 ^ (h3 << 1)) << 1;
        }
    };
}

#endif //CATAN_NODECOORDS_HPP