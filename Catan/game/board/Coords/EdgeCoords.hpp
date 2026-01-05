//
// Created by matija on 1/4/26.
//

#ifndef CATAN_EDGECOORDS_HPP
#define CATAN_EDGECOORDS_HPP
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/ElementCoords.hpp>

class EdgeCoords : public ElementCoords{
private:
    EdgeDirection oppositeDir() const;
public:
    using ElementCoords::ElementCoords;
    EdgeCoords(AxialCoords ax, EdgeDirection dir) : ElementCoords(ax.q(),ax.r(), static_cast<int>(dir)) {}
    EdgeCoords standardize() const;
    EdgeCoords next() const;
    EdgeDirection direction() const {return static_cast<EdgeDirection>(m_i);}
    NodeCoords getNodeCoordAfter();
    NodeCoords getNodeCoordBefore();
    friend bool operator==(const EdgeCoords &lhs, const EdgeCoords &rhs) {
        return static_cast<const ElementCoords &>(lhs) == static_cast<const ElementCoords &>(rhs);
    }
    friend bool operator!=(const EdgeCoords &lhs, const EdgeCoords &rhs) {
        return !(lhs == rhs);
    }
};


namespace std {
    template <>
    struct hash<EdgeCoords> {
        std::size_t operator()(const EdgeCoords& a) const noexcept {
            auto aa=a.standardize();
            std::size_t h1 = std::hash<int>{}(aa.getAxialCoords().q());
            std::size_t h2 = std::hash<int>{}(aa.getAxialCoords().r());
            std::size_t h3 = std::hash<int>{}(static_cast<int>(aa.direction()));
            return h1 ^ ((h2 ^ (h3 << 1)) << 1);
        }
    };
}

#endif //CATAN_EDGECOORDS_HPP