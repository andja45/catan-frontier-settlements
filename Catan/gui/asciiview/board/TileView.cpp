//
// Created by matija on 12/20/25.
//

#include "TileView.hpp"

#include <board/BoardView.hpp>
#include <board/Tile.h>
#include "BoardTypes.hpp"
#include <types/TypeAliases.h>

char TileView::resourceToChar( const BoardTheme& theme) const {
    switch (m_tile->getResourceType()) {
        case ResourceType::Wood: return theme.woodChar;
        case ResourceType::Brick: return theme.brickChar;
        case ResourceType::Ore: return theme.oreChar;
        case ResourceType::Wool: return theme.woolChar;
        case ResourceType::Wheat: return theme.wheatChar;
        case ResourceType::Desert: return theme.desertChar;
        case ResourceType::Sea: return theme.seaChar;
        default: return ' ';
    }
}

void TileView::draw(Canvas &canvas, const BoardTheme &theme) const {
    int rows=m_size.second-2;
    int max_width=m_size.first-2;
    //int min_width=max_width-2*rows/2;
    int margin=2;
    //int cols=min_width-margin*2;
    int spacing=rows/2;

    int x= m_pos.first-(max_width-1)/2;
    int y=m_pos.second-rows/2;

    for (int r=0; r<rows; r++) {
        int c;
        for (c=spacing; c<spacing+margin; c++) {
            canvas[x+c][y+r]={' ',1};
        }
        for (; c<max_width-margin-spacing; c++) {
            canvas[x+c][y+r]={resourceToChar(theme),1};
        }
        spacing-=2;
        spacing=abs(spacing);
        //cols=max_width-(spacing+margin)*2;
    }

    for (int i = -1; i <= 2; ++i) {
        canvas[m_pos.first+i][m_pos.second]={' ',1};
    }
    if (!m_tile->isRobberOnTile()) {
        canvas[m_pos.first][m_pos.second]   ={(char)('0'+m_tile->getNumber()/10),1};
        canvas[m_pos.first+1][m_pos.second] ={(char)('0'+m_tile->getNumber()%10),1};
    }
    else {
        canvas[m_pos.first][m_pos.second]   ={theme.robberChar,1};
        canvas[m_pos.first+1][m_pos.second] ={theme.robberChar,1};

    }
}

std::vector<std::pair<NodeAsciiDirection, ScreenCoords> > TileView::getNodes(ScreenCoords m_pos, ScreenCoords m_size) {

    auto [width,height]=m_size;
    auto [x,y]=m_pos;

    int halfStepY=height/2;
    int halfStepX=width/2;
    int sideStepX=halfStepX-BoardView::slopeWidthForHeight(height)+1;

    return {
        {NodeAsciiDirection::Left,{x-halfStepX+1,y}},
        {NodeAsciiDirection::BottomLeft,{x-sideStepX+1,y+halfStepY}},
        {NodeAsciiDirection::BottomRight,{x+sideStepX,y+halfStepY}},
        {NodeAsciiDirection::Right,{x+halfStepX,y}},
        {NodeAsciiDirection::TopRight,{x+sideStepX,y-halfStepY}},
        {NodeAsciiDirection::TopLeft,{x-sideStepX+1,y-halfStepY}},
    };

}
