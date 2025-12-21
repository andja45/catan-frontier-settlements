//
// Created by matija on 12/20/25.
//

#include "TileView.hpp"

char TileView::resourceToChar( const BoardTheme& theme) const {
    switch (m_tile->getType()) {
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
            canvas[x+c][y+r]={resourceToChar(m_tile->getType(), theme),1};
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

std::vector<ScreenCoords> TileView::getNodes() {
    std::vector<ScreenCoords> coords;
    int rows=m_size.second;
    int max_width=m_size.first;

    int x= m_pos.first-(max_width-1)/2;
    int y=m_pos.second-rows/2;

    int spacing=rows/2;

    coords.emplace_back(x+spacing,y);
    coords.emplace_back(x+max_width-1-spacing,y);

    coords.emplace_back(x,m_pos.second);
    coords.emplace_back(x+max_width-1,m_pos.second);

    coords.emplace_back(x+spacing,y+rows-1);
    coords.emplace_back(x+max_width-1-spacing,y+rows-1);

    return coords;
}
