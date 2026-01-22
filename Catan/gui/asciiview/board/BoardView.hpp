//
// Created by matija on 12/20/25.
//

#ifndef CATAN_BOARDVIEW_HPP
#define CATAN_BOARDVIEW_HPP
#include <utility>
#include <vector>

#include <board/Board.h>
#include <drawing/AsciiComposite.hpp>
#include <drawing/AsciiDrawable.hpp>

#include "TileView.hpp"
#include "EdgeView.hpp"
#include "NodeView.hpp"
class Tile;
class BoardView : public AsciiDrawable{ // TODO change to composite
private:
    std::vector<TileView> m_tiles;
    std::vector<EdgeView> m_edges;
    std::vector<NodeView> m_nodes;


    static ScreenCoords constexpr sizeBig={18,7};
    static ScreenCoords constexpr sizeSmall={12,5};

    Board* m_board;

    ScreenSize m_tileSize;
    ScreenCoords m_center;
    ScreenSize m_margin;


    inline static constexpr int minTileWidth=8;
    inline static constexpr int minTileHeight=5;

    // because its inconvenient to draw hexes point side up in ascii we flip offset coords, we chose odd row indent offset coords
    // note board will appear flipped
    static OffsetCoords axialToFlippedOffset(TileCoords axial);
    static ScreenCoords offsetToScreen(TileCoords offset, ScreenSize tileSize, ScreenCoords origin = {0, 0});

    static ScreenCoords stepSize(ScreenSize tileSize); // step to reach next hex


    void computeSizes();
    void processTileCords(TileCoords tileCoords, std::vector<std::pair<NodeAsciiDirection,ScreenCoords>> &cords);
    static NodeDirection fromNodeAsciiDir(NodeAsciiDirection dir);
    static EdgeAsciiDirection toEdgeAsciiDir(EdgeDirection dir);



public:
    virtual ~BoardView() = default;

    BoardView(Board* board, ScreenSize gridSize):m_board(board) {
        setTileSize(gridSize);
        reorganize();
    }
    void setTileSize(ScreenSize size);
    void setMargin(ScreenSize margin) {m_margin=margin;}
    void reorganize();
    void fitToScreen(ScreenCoords scr, bool stretch);

    static int slopeWidthForHeight(int height){return height/2+1;} //edges including
    static int maxHeightForWidth(int width) { // in order to reach top with slopes
        int maxSlopeCount=(width-2)/2;
        return maxSlopeCount*2-1;
    }
    static int minWidthForHeight(int height){return slopeWidthForHeight(height)*2 +2;} // +2 for tile number
    void render(Canvas &canvas) const override;
};

#endif //CATAN_BOARDVIEW_HPPHPPHPP