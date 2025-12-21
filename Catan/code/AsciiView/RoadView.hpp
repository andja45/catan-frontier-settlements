//
// Created by matija on 12/20/25.
//

#ifndef CATAN_ROADVIEW_HPP
#define CATAN_ROADVIEW_HPP
#include "BoardView.hpp"

class RoadView {
private:
    Edge* m_edge;

    ScreenCoords m_start;
    ScreenCoords m_end;
    PointDirection m_side;

    char sideToChar(const BoardTheme &theme)const;
public:
    RoadView(Edge* edge, const ScreenCoords &start, const ScreenCoords &end, PointDirection side) : m_edge(edge), m_start(start), m_end(end), m_side(side) {}
    void draw(Canvas &canvas, const BoardTheme &theme) const;
};

#endif //CATAN_ROADVIEW_HPP