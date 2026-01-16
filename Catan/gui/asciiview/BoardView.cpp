//
// Created by matija on 12/20/25.
//

#include "BoardView.hpp"
#include <iostream>
#include <limits>

#include <board/Board.h>
#include <board/Tile.h>



void BoardView::computeSizes() {
    int minQ,maxQ,minR,maxR;
    int minRow,maxRow,minCol,maxCol;

    minR=minCol=minRow=minQ=std::numeric_limits<int>::max();
    maxR=maxCol=maxRow=maxQ=std::numeric_limits<int>::min();

    for (auto c: m_board->getBoardCords()) {
        OffsetCoords offsetCord = axialToFlippedOffset(c);

        minQ=std::min(minQ,offsetCord.q());
        maxQ=std::max(maxQ,offsetCord.q());
        minR=std::min(minR,offsetCord.r());
        maxR=std::max(maxR,offsetCord.r());

        ScreenCoords pos=offsetToScreen(offsetCord,m_tileSize);
        for (auto [_,n]:TileView::getNodes(pos,m_tileSize)) {
            minCol=std::min(minRow,n.first);
            maxCol=std::max(maxRow,n.first);
            minRow=std::min(minCol,n.second);
            maxRow=std::max(maxCol,n.second);
        }
    }

    int rows=maxR-minR+1;
    int cols=maxQ-minQ+1;

    m_canvasSize={maxCol-minCol+1,maxRow-minRow+1}; // width height
    m_center={-minCol+m_margin.first,-minRow+m_margin.second};
}


NodeDirection BoardView::fromNodeAsciiDir(NodeAsciiDirection dir) {
    return directions::flipOrientation(dir);
}

EdgeDirection BoardView::fromEdgeAsciiDir(EdgeAsciiDirection dir) {
    return directions::flipOrientation(dir);
}

void BoardView::fitToScreen(ScreenCoords scr, bool stretch) {
    m_margin={0,0};
    m_tileSize={12,5};

    auto [scrWidth,scrHeight]=scr;
    auto [bWidth,bHeight]=m_canvasSize;

    computeSizes();
    float rowScale=scrHeight/static_cast<float>(bHeight);
    float colScale=scrWidth/static_cast<float>(bWidth);
        
    if (stretch) {
        setTileSize({static_cast<int>(m_tileSize.first*colScale),static_cast<int>(m_tileSize.second*rowScale)});
    }
    else {
        float fitScale=std::min(rowScale,colScale);
        setTileSize({static_cast<int>(m_tileSize.first*fitScale),static_cast<int>(m_tileSize.second*fitScale)});
    }
}

void BoardView::processTileCords(TileCoords tileCoords, std::vector<std::pair<NodeAsciiDirection,ScreenCoords>> &cords) {
        // .
        for (auto [nodeAsciiDir,scrCoord]: cords) {
            NodeDirection dir=fromNodeAsciiDir(nodeAsciiDir);
            NodeCoords nc={tileCoords,dir};

        }

        int i_next=(i+1)%static_cast<int>(PointDirection::End);
        NodeCoords nc={tileCoords,static_cast<NodeDirection>(i)};
        EdgeCoords ec={tileCoords,static_cast<EdgeDirection>(j)};


        Edge* edge=m_board->getEdgeAt(ec);
        Node* node=m_board->getNodeAt(nc);
        NodeView nw(node,cords[i]);
        PointDirection dir=static_cast<PointDirection>((-j+startEdgeDir)%dirCount);
        EdgeView ew(edge,cords[i],cords[i_next],dir); //.

        m_edges.push_back(std::move(ew));
        m_nodes.push_back(std::move(nw));

    }

}


void BoardView::reorganize() {
    m_tiles.clear();
    m_edges.clear();
    m_nodes.clear();
    m_cells.clear();

    computeSizes();
    m_cells.resize(m_canvasSize.first,std::vector<Cell>(m_canvasSize.second,{' ',0}));


    for (auto c: m_board->getBoardCords()) {
        TileCoords offsetCord=axialToFlippedOffset(c);
        ScreenCoords pos=offsetToScreen(offsetCord,m_tileSize,m_center);

        TileView tw=TileView(m_board->getTileAt(c),pos,m_tileSize);
        m_tiles.push_back(std::move(tw));

        auto coords= TileView::getNodes(pos,m_tileSize);
        processTileCords(m_board->getTileAt(c)->getTileCoord(),coords);
    }
}

OffsetCoords BoardView::axialToFlippedOffset(TileCoords axial) {
    int offsetR=axial.r();
    int offsetQ=axial.q()+(axial.r()-axial.q()%2)/2;
    return {offsetR,offsetQ};
}

ScreenCoords BoardView::stepSize(ScreenCoords tileSize) {
    auto [tileWidth,tileHeight]=tileSize;
    int slopeWidth=slopeWidthForHeight(tileHeight);
    int middleWidth=tileWidth-slopeWidth*2;
    int stepX=middleWidth/2+slopeWidth+middleWidth/2;
    int stepY=tileHeight-1;

    return {stepX,stepY};
}

ScreenCoords BoardView::offsetToScreen(TileCoords offset,ScreenCoords tileSize,ScreenCoords origin) {
    auto [stepX,stepY]=stepSize(tileSize);
    int col=offset.q()*stepX;
    int row=offset.r()*stepY;
    if (offset.q()%2)
        row+=stepY/2;
    return {origin.first+col,origin.second+row};
}

void BoardView::renderBoard() {
    for (auto n:m_nodes) {
        n.draw(m_cells,m_theme);
    }
    for (auto e:m_edges) {
        e.draw(m_cells,m_theme);
    }
    for (auto t:m_tiles) {
        t.draw(m_cells,m_theme);
    }
}

void BoardView::printCell(Cell c, std::ostream& os) {
    os<<c.ch;
}

void BoardView::blitBoard(std::ostream& os=std::cout) {
    for (int x=0; x<m_canvasSize.first+m_margin.first*2+2; x++) {
        os<<'-';
    }
    for (int y=0; y<m_margin.second; y++) {
        os<<std::endl;
    }
    for (int y=0; y<m_canvasSize.second; y++) {
        os<<'|';
        for (int x=0; x<m_margin.first; x++) {
            os<<' ';
        }
        for (int x=0; x<m_canvasSize.first; x++) {
            printCell(m_cells[x][y],os);
        }
        for (int x=0; x<m_margin.first; x++) {
            os<<' ';
        }
        os<<'|';
        os<<std::endl;
    }
    for (int y=0; y<m_margin.second; y++) {
        os<<std::endl;
    }
    for (int x=0; x<m_canvasSize.first+m_margin.first*2+2; x++) {
        os<<'-';
    }
}

void BoardView::drawBoard(std::ostream& os=std::cout) {
    os<<"Board:"<<std::endl;
    renderBoard();
    blitBoard(os);
}

void BoardView::setTileSize(ScreenCoords size) {
    int width=size.first/2*2; // always even number
    int height=(size.second-1)/2*2+1; // always odd number
    height=std::min(maxHeightForWidth(width),height);

    width=std::max(width,minTileWidth);
    height=std::max(height,minTileHeight);
    m_tileSize={width,height};

}
