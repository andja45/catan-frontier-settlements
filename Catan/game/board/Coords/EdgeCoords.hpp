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
};

#endif //CATAN_EDGECOORDS_HPP