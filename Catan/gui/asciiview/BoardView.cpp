//
// Created by matija on 12/20/25.
//

#include "BoardView.hpp"
#include <iostream>

#include <board/Board.h>
#include <board/Tile.h>



void BoardView::computeSizes() {
    int minX,maxX,minY,maxY;
    for (auto c: m_board->getBoardCords()) {
        TileCoords offsetCord=axialToOffset(c);

        minX=std::min(minX,offsetCord.first);
        maxX=std::max(maxX,offsetCord.first);
        minY=std::min(minY,offsetCord.second);
        maxY=std::max(maxY,offsetCord.second);
    }

    int rows=maxY-minY+1;
    int cols=maxX-minX+1;

    ScreenCoords origin={(m_gridSize.first-1)/2,(m_gridSize.second-1)/2};
    ScreenCoords margin={m_gridSize.first,m_gridSize.second};
    m_canvasSize=offsetToScreen({cols,rows},margin,m_gridSize);

    m_cells=std::vector(m_canvasSize.first,std::vector<Cell>(m_canvasSize.second));
    m_center=offsetToScreen({rows/2,cols/2},origin,m_gridSize);
}

void BoardView::processTileCords(Tile* tile, std::vector<ScreenCoords> &cords) {
    for (int i = 0,j=0; i < static_cast<int>(PointDirection::End) && j< static_cast<int>(SideDirection::End); ++i,++j) {
        int i_next=(i+1)%static_cast<int>(PointDirection::End);
        PointDirection edgeDir=static_cast<PointDirection>(j);

        Node* n=tile->getNodeAt(i_next);
        Edge* e=tile->getEdgeAt(j);
        NodeView nw(n,cords[i]);
        EdgeView ew(e,cords[i],cords[i_next],edgeDir);

        m_edges.push_back(std::move(ew));
        m_nodes.push_back(std::move(nw));
    }
}


void BoardView::init() {

    computeSizes();

    for (auto c: m_board->getBoardCords()) {
        TileCoords offsetCord=axialToOffset(c);
        ScreenCoords pos=offsetToScreen({offsetCord.first,offsetCord.second},m_center,m_gridSize);

        TileView tw=TileView(m_board->getTileAt(c),pos,m_gridSize);
        m_tiles.push_back(std::move(tw));

        auto coords= tw.getNodes();
        processTileCords(m_board->getTileAt(c),coords);

    }
}

TileCoords BoardView::axialToOffset(TileCoords axial) {
    auto [q,r]=axial;
    int offsetX=q;
    int offsetY=r+(q-q%2)/2;
    return {offsetX,offsetY};
}

ScreenCoords BoardView::stepSize(ScreenCoords tileSize) {
    int stepX=tileSize.first/2+(tileSize.first-tileSize.second+1)/2-1;
    int stepY=tileSize.second-1;

    return {stepX,stepY};
}

ScreenCoords BoardView::offsetToScreen(TileCoords offset,ScreenCoords origin ,ScreenCoords tileSize) {
    auto [stepX,stepY]=stepSize(tileSize);
    int x=offset.first*stepX;
    int y=offset.second*stepY+stepY/2*(offset.first%2);
    return {origin.first+x,origin.second+y};
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
    for (auto line:m_cells) {
        for (auto cell:line) {
            printCell(cell,os);
        }
        os<<std::endl;
    }
}

void BoardView::drawBoard(std::ostream& os=std::cout) {
    renderBoard();
    blitBoard(os);
}
