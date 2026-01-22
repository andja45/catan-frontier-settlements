//
// Created by matija on 12/20/25.
//

#include "EdgeView.hpp"
#include <board/Board.h>
#include <drawing/AsciiCanvas.hpp>
#include <drawing/AsciiTheme.hpp>

char EdgeView::sideToChar(const BoardTheme &theme) const {
    switch (m_side) {
    case PointDirection::Top:
    case PointDirection::Bottom: return theme.upDownEdgeChar;

    case PointDirection::RightTop:
    case PointDirection::LeftBottom: return theme.upRightDownLeftEdgeChar;
    case PointDirection::RightBottom:
    case PointDirection::LeftTop: return theme.upLeftDownRightEdgeChar;
    case PointDirection::End: return ' ';
    }
    return '\0';
}

void EdgeView::render(Canvas &canvas) const {
    const auto& theme=BoardTheme::getInstance();
    int step_x=m_end.x-m_start.x;
    int step_y=m_end.y-m_start.y;
    int len=std::max(std::abs(step_x)-1, std::abs(step_y)-1);
    step_x/=std::abs(step_x);
    if (step_y!=0)
        step_y/=std::abs(step_y);

    for (int i=1; i<=len; i++) {
        canvas.setCell({i*step_x,i*step_y},{sideToChar(theme),1});
    }
}
