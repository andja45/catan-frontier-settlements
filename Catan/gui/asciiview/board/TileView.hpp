//
// Created by matija on 12/20/25.
//

#ifndef CATAN_TILEVIEW_HPP
#define CATAN_TILEVIEW_HPP
#include <vector>
#include <drawing/AsciiDrawable.hpp>
#include <drawing/AsciiTheme.hpp>
#include <types/TypeAliases.h>

#include <types/TypeAliases.h>

#include "../drawing/AsciiTypes.hpp"

class Tile;

class TileView : public AsciiDrawable{
private:
    Tile* m_tile;
    ScreenSize m_size;
    ScreenCoords m_pos;

    char resourceToChar(const BoardTheme &theme)const;
public:
    TileView(Tile* tile, ScreenCoords pos,  ScreenSize size) : AsciiDrawable({pos.x-size.width/2,pos.y-size.height/2},size), m_tile(tile), m_size(size), m_pos(pos) {}
    void render(Canvas &canvas) const;
    static std::vector<std::pair<NodeAsciiDirection, ScreenCoords>> getNodes(ScreenCoords pos, ScreenSize size);
};

#endif //CATAN_TILEVIEW_HPP