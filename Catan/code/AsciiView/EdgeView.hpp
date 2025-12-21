//
// Created by matija on 12/20/25.
//

#ifndef CATAN_ROADVIEW_HPP
#define CATAN_ROADVIEW_HPP

enum class PointDirection;
class Edge;
#include "ViewTypes.hpp"

class EdgeView {
private:
    Edge* m_edge;

    ScreenCoords m_start;
    ScreenCoords m_end;
    PointDirection m_side;

    char sideToChar(const BoardTheme &theme)const;
public:
    EdgeView(Edge* edge, ScreenCoords start,  ScreenCoords end, PointDirection side) : m_edge(edge), m_start(start), m_end(end), m_side(side) {}
    void draw(Canvas &canvas, const BoardTheme &theme) const;
};

#endif //CATAN_ROADVIEW_HPP