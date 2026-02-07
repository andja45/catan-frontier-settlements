//
// Created by matija on 12/20/25.
//

#ifndef CATAN_ROADVIEW_HPP
#define CATAN_ROADVIEW_HPP
#include <drawing/AsciiDrawable.hpp>
#include <drawing/AsciiTheme.hpp>

enum class PointDirection;
class Edge;
#include "../drawing/AsciiTypes.hpp"

class EdgeView : public AsciiDrawable{
private:
    Edge* m_edge;

    ScreenCoords m_start;
    ScreenCoords m_end;
    PointDirection m_side;

    char sideToChar(const BoardTheme &theme)const;
public:
    EdgeView(Edge* edge, ScreenCoords start,  ScreenCoords end, PointDirection side) : AsciiDrawable(start,{std::abs(end.x-start.x),std::abs(end.y-start.y)})
      , m_edge(edge)
      , m_start(start)
      , m_end(end)
      , m_side(side) {
    }

    void render(Canvas &canvas) const;
};

#endif //CATAN_ROADVIEW_HPP