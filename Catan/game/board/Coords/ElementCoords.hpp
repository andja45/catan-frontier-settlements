//
// Created by matija on 1/4/26.
//

#ifndef CATAN_ELEMENTCOORDS_HPP
#define CATAN_ELEMENTCOORDS_HPP
#include <board/Coords/AxialCoords.hpp>

class ElementCoords {
protected:
    int m_q;
    int m_r;
    int m_i;

public:
    ElementCoords(int q, int r, int i) : m_q(q), m_r(r), m_i(i) {}
    AxialCoords getAxialCoords() const {return {m_q,m_r};}
};

#endif //CATAN_ELEMENTCOORDS_HPP