//
// Created by matija on 1/4/26.
//

#ifndef CATAN_NODECOORDS_HPP
#define CATAN_NODECOORDS_HPP
#include <board/Coords/AxialCoords.hpp>
#include <board/Coords/ElementCoords.hpp>

class NodeCoords:ElementCoords {
public:
    using ElementCoords::ElementCoords;
    NodeCoords(AxialCoords ax, NodeDirection dir) : ElementCoords(ax.q(),ax.r(), static_cast<int>(dir)) {}
    NodeCoords standardize() const;
    NodeCoords next() const;
    NodeDirection direction() const {return static_cast<NodeDirection>(m_i);}

};

#endif //CATAN_NODECOORDS_HPP