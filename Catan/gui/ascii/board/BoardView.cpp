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
            minCol=std::min(minCol,n.x);
            maxCol=std::max(maxCol,n.x);
            minRow=std::min(minRow,n.y);
            maxRow=std::max(maxRow,n.y);
        }
    }

    int rows=maxR-minR+1;
    int cols=maxQ-minQ+1;

    m_size={maxCol-minCol+1,maxRow-minRow+1}; // width height
    m_center={-minCol+m_margin.width,-minRow+m_margin.height};
}


NodeDirection BoardView::fromNodeAsciiDir(NodeAsciiDirection dir) {
    return directions::flipOrientation(dir);
}

EdgeAsciiDirection BoardView::toEdgeAsciiDir(EdgeDirection dir) {
    return directions::flipOrientation(dir);
}

void BoardView::fitToScreen(ScreenCoords scr, bool stretch) {
    m_margin={0,0};
    m_tileSize={12,5};

    auto [scrWidth,scrHeight]=scr;
    auto [bWidth,bHeight]=m_size;

    computeSizes();
    float rowScale=scrHeight/static_cast<float>(bHeight);
    float colScale=scrWidth/static_cast<float>(bWidth);

    if (stretch) {
        setTileSize({static_cast<int>(m_tileSize.width*colScale),static_cast<int>(m_tileSize.height*rowScale)});
    }
    else {
        float fitScale=std::min(rowScale,colScale);
        setTileSize({static_cast<int>(m_tileSize.width*fitScale),static_cast<int>(m_tileSize.height*fitScale)});
    }
}

void BoardView::render(Canvas &canvas) const {
    const auto& theme=BoardTheme::getInstance();
    for (auto n:m_nodes) {
        n.draw(canvas);
    }
    for (auto e:m_edges) {
        e.draw(canvas);
    }
    for (auto t:m_tiles) {
        t.draw(canvas);
    }
}

void BoardView::processTileCords(TileCoords tileCoords, std::vector<std::pair<NodeAsciiDirection,ScreenCoords>> &cords) {
    // .
    for (int i=0; i<cords.size(); i++) {
        auto [nodeAsciiDir,scrCoord] = cords[i];
        auto [_,nextScrCoords] = cords[(i+1)%cords.size()];
        NodeDirection dir=fromNodeAsciiDir(nodeAsciiDir);
        NodeCoords nc={tileCoords,dir};

        NodeView nv(m_board->getNodeAt(nc),scrCoord);
        m_nodes.push_back(std::move(nv));

        EdgeDirection sd=directions::nextSide(dir);
        EdgeAsciiDirection ed=toEdgeAsciiDir(sd);
        EdgeCoords ec={tileCoords,sd};
        Edge* edge=m_board->getEdgeAt(ec);
        EdgeView ew(edge,scrCoord,nextScrCoords,ed); //.
        m_edges.push_back(std::move(ew));
    }
}



void BoardView::reorganize() {
    m_tiles.clear();
    m_edges.clear();
    m_nodes.clear();

    computeSizes();


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
    int offsetQ=axial.q()+(axial.r()-(axial.r()&1))/2;
    return {offsetR,offsetQ};
}

ScreenCoords BoardView::stepSize(ScreenSize tileSize) {
    auto [tileWidth,tileHeight]=tileSize;
    int slopeWidth=slopeWidthForHeight(tileHeight);
    int middleWidth=tileWidth-slopeWidth*2;
    int stepX=middleWidth/2+slopeWidth+middleWidth/2;
    int stepY=tileHeight-1;

    return {stepX,stepY};
}

ScreenCoords BoardView::offsetToScreen(TileCoords offset,ScreenSize tileSize,ScreenCoords origin) {
    auto [stepX,stepY]=stepSize(tileSize);
    int col=offset.q()*stepX;
    int row=offset.r()*stepY;
    if (offset.q()%2)
        row+=stepY/2;
    return {origin.x+col,origin.y+row};
}

void BoardView::setTileSize(ScreenSize size) {
    int width=size.width /2*2; // always even number
    int height=(size.height-1)/2*2+1; // always odd number
    height=std::min(maxHeightForWidth(width),height);

    width=std::max(width,minTileWidth);
    height=std::max(height,minTileHeight);
    m_tileSize={width,height};

}
