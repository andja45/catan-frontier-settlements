//
// Created by matija on 12/20/25.
//

#ifndef CATAN_TILEVIEW_HPP
#define CATAN_TILEVIEW_HPP
#include "BoardView.hpp"

class Tile;

class TileView {
private:
    Tile* m_tile;
    ScreenCoords m_size;
    ScreenCoords m_pos;

    char resourceToChar(const BoardTheme &theme)const;
public:
    TileView(Tile* tile, const ScreenCoords &pos, const ScreenCoords &size) : m_tile(tile), m_size(size), m_pos(pos) {}
    void draw(Canvas &canvas, const BoardTheme &theme) const;
    std::vector<ScreenCoords> getNodes();
};

#endif //CATAN_TILEVIEW_HPP