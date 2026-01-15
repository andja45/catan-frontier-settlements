//
// Created by matija on 12/20/25.
//

#ifndef CATAN_BOARDVIEW_HPP
#define CATAN_BOARDVIEW_HPP
#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include <board/Board.h>
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
    ScreenCoords m_tileSize;
    ScreenCoords m_center;
    ScreenCoords m_margin;

    //std::map<int,std::pair<int,int>> m_computedTilePositions;
    //void drawTile(Tile* tile, std::pair<int,int> pos);



    // because its inconvenient to draw hexes point side up in ascii we flip offset coords, we chose odd row indent offset coords
    // note board will appear flipped
    static int slopeWidthForHeight(int height){return height/2+1;} //edges including
    static int maxHeightForWidth(int width) { // in order to reach top with slopes
        int maxSlopeCount=(width-2)/2;
        return maxSlopeCount*2-1;
    }
    static int minWidthForHeight(int height){return slopeWidthForHeight(height)*2 +2;} // +2 for tile number

    inline static constexpr int minTileWidth=8;
    inline static constexpr int minTileHeight=5;

    static OffsetCoords axialToFlippedOffset(TileCoords axial);
    static ScreenCoords offsetToScreen(TileCoords offset, ScreenCoords tileSize,  ScreenCoords origin={0,0});

    static ScreenCoords stepSize(ScreenCoords tileSize); // step to reach next hex

    void renderBoard();

    static void printCell(Cell c, std::ostream &os);

    virtual void blitBoard(std::ostream &os);

    void computeSizes();
    void processTileCords(TileCoords tileCoords, std::vector<ScreenCoords> &cords);
    SideDirection pointToSide(PointDirection dir);
    PointDirection sideToPoint(SideDirection dir);

public:
    virtual ~BoardView() = default;

    void drawBoard(std::ostream &os);
    BoardView(Board* board, BoardTheme theme, ScreenCoords gridSize, ScreenCoords margin={0,0}):m_board(board),m_theme(theme) {
        setTileSize(gridSize);
        reorganize();
    }
    void setTileSize(ScreenCoords size);
    void setMargin(ScreenCoords margin) {m_margin=margin;}
    void reorganize();
    void fitToScreen(ScreenCoords scr, bool stretch);
};

#endif //CATAN_BOARDVIEW_HPP