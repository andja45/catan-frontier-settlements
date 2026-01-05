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
    ElementCoords() = default;
    ElementCoords(int q, int r, int i) : m_q(q), m_r(r), m_i(i) {}
    AxialCoords getAxialCoords() const {return {m_q,m_r};}

    friend bool operator==(const ElementCoords &lhs, const ElementCoords &rhs) {
        return lhs.m_q == rhs.m_q
               && lhs.m_r == rhs.m_r
               && lhs.m_i == rhs.m_i;
    }

    friend bool operator!=(const ElementCoords &lhs, const ElementCoords &rhs) {
        return !(lhs == rhs);
    }
};

#endif //CATAN_ELEMENTCOORDS_HPP