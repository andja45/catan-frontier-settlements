//
// Created by matija on 1/4/26.
//

#ifndef CATAN_EDGECOORDS_HPP
#define CATAN_EDGECOORDS_HPP
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/ElementCoords.hpp>

class EdgeCoords : public ElementCoords{
private:
public:
    using ElementCoords::ElementCoords;
    EdgeCoords(AxialCoords ax, EdgeDirection dir);
    EdgeCoords(int q, int r, int i) : EdgeCoords({q,r},static_cast<EdgeDirection>(i)) {}

};


namespace std {
    template <>
    struct hash<EdgeCoords> {
        std::size_t operator()(const EdgeCoords& a) const noexcept {
            return a.elementHash();
        }
    };
}

#endif //CATAN_EDGECOORDS_HPP