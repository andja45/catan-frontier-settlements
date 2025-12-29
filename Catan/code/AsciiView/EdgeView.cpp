//
// Created by matija on 12/20/25.
//

#include "EdgeView.hpp"

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
}

void EdgeView::draw(Canvas &canvas, const BoardTheme &theme) const {
    int step_x=m_end.first-m_start.first;
    int step_y=m_end.second-m_start.second;
    int len=std::max(std::abs(step_x-2), std::abs(step_y-2));
    step_x/=std::abs(step_x);
    step_y/=std::abs(step_y);

    for (int i=1; i<=len; i++) {
        canvas[m_start.first+i*step_x][m_start.second+i*step_y]= {sideToChar(theme),1};
    }
}