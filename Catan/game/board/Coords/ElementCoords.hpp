//
// Created by matija on 1/4/26.
//

#ifndef CATAN_ELEMENTCOORDS_HPP
#define CATAN_ELEMENTCOORDS_HPP
#include <board/Coords/AxialCoords.hpp>

// small shared base for edge and node coords
class ElementCoords {
protected:
    int m_q;
    int m_r;
    int m_i;
public:
    ElementCoords() = default;
    size_t elementHash() const {
        return std::hash<int>{}(m_i)^(std::hash<int>{}(m_q) ^ (std::hash<int>{}(m_r) << 1)<<1);
    }
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