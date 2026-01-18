//
// Created by matija on 12/20/25.
//

#include "TileView.hpp"

#include <board/BoardView.hpp>
#include <board/Tile.h>
#include "../drawing/AsciiTypes.hpp"
#include <types/TypeAliases.h>
#include <drawing/AsciiTheme.hpp>
#include <drawing/AsciiCanvas.hpp>

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

void TileView::render(Canvas &canvas) const {
    const auto& theme=BoardTheme::getInstance();

    int rows=m_size.height-2;
    int max_width=m_size.width-2;
    //int min_width=max_width-2*rows/2;
    int margin=2;
    //int cols=min_width-margin*2;
    int spacing=rows/2;

    int x= m_pos.x-(max_width-1)/2;
    int y=m_pos.y-rows/2;

    for (int r=0; r<rows; r++) {
        int c;
        for (c=spacing; c<spacing+margin; c++) {
            canvas.setCell({x+c,y+r},{' ',1});
        }
        for (; c<max_width-margin-spacing; c++) {
            canvas.setCell({x+c,y+r},{resourceToChar(theme),1});
        }
        spacing-=2;
        spacing=abs(spacing);
        //cols=max_width-(spacing+margin)*2;
    }

    for (int i = -1; i <= 2; ++i) {
        canvas.setCell({m_pos.x+i,m_pos.y},{' ',1});
    }
    if (!m_tile->isRobberOnTile()) {
        canvas.setCell({m_pos.x,m_pos.y},{(char)('0'+m_tile->getNumber()/10),1});
        canvas.setCell({m_pos.x,m_pos.y},{(char)('0'+m_tile->getNumber()%10),1});
    }
    else {
        canvas.setCell({m_pos.x,m_pos.y},{theme.robberChar,1});
        canvas.setCell({m_pos.x+1,m_pos.y},{theme.robberChar,1});

    }
}

std::vector<std::pair<NodeAsciiDirection, ScreenCoords> > TileView::getNodes(ScreenCoords pos, ScreenSize size) {

    auto [width,height]=size;
    auto [x,y]=pos;

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
