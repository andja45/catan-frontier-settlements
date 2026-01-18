//
// Created by matija on 12/20/25.
//

#ifndef CATAN_TILEVIEW_HPP
#define CATAN_TILEVIEW_HPP
#include <vector>
#include <types/TypeAliases.h>

#include "ViewTypes.hpp"

class Tile;

class TileView {
private:
    Tile* m_tile;
    ScreenCoords m_size;
    ScreenCoords m_pos;

    char resourceToChar(const BoardTheme &theme)const;
public:
    TileView(Tile* tile, ScreenCoords pos,  ScreenCoords size) : m_tile(tile), m_size(size), m_pos(pos) {}
    void draw(Canvas &canvas, const BoardTheme &theme) const;
    static std::vector<std::pair<NodeAsciiDirection, ScreenCoords>> getNodes(ScreenCoords m_pos, ScreenCoords m_size);
};

#endif //CATAN_TILEVIEW_HPP