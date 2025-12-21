//
// Created by matija on 12/20/25.
//

#ifndef CATAN_BOARDVIEW_HPP
#define CATAN_BOARDVIEW_HPP
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "../../headers/Board/Board.h"
#include "ViewTypes.hpp"
#include "TileView.hpp"
#include "EdgeView.hpp"
#include "NodeView.hpp"
class Tile;
class BoardView {
private:
    std::vector<TileView> m_tiles;
    std::vector<EdgeView> m_edges;
    std::vector<NodeView> m_nodes;


    static ScreenCoords constexpr sizeBig={18,7};
    static ScreenCoords constexpr sizeSmall={12,5};

    Board* m_board;
    BoardTheme m_theme;

    Canvas m_cells;

    ScreenCoords m_canvasSize;
    ScreenCoords m_gridSize;
    ScreenCoords m_center;

    //std::map<int,std::pair<int,int>> m_computedTilePositions;
    //void drawTile(Tile* tile, std::pair<int,int> pos);

    void computeSizes();

    void processTileCords(Tile *tile, std::vector<ScreenCoords> &cords);

    void init();

    static HexCoords axialToOffset(HexCoords axial);
    static ScreenCoords stepSize(ScreenCoords tileSize);
    static ScreenCoords offsetToScreen(HexCoords offset, ScreenCoords origin, ScreenCoords tileSize);

    void renderBoard();

    void printCell(Cell c, std::ostream &os);

    virtual void blitBoard(std::ostream &os);
public:
    void drawBoard(std::ostream &os);
    BoardView(Board* board, BoardTheme theme):m_board(board),m_theme(theme){}
    void setGridSize(ScreenCoords size){m_gridSize=size;}
};

#endif //CATAN_BOARDVIEW_HPP