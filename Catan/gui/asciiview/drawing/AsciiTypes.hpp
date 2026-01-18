//
// Created by matija on 12/21/25.
//

#ifndef CATAN_VIEWTYPES_HPP
#define CATAN_VIEWTYPES_HPP

#include <utility>
#include <vector>
#include <cstdint>

class AsciiCanvas;

struct Size {
    int width=0;
    int height=0;
};
struct Coords {
    int x=0;
    int y=0;
    Coords operator+(const Size& size) const {
        return Coords{x + size.width, y + size.height};
    }
    Coords operator+(const Coords& other) const {
        return Coords{x + other.x, y + other.y};
    }
};
struct Cell { char ch; std::uint8_t color; };

using CellSpan = std::vector<std::vector<Cell>>;
using Canvas=AsciiCanvas;
using ScreenCoords=Coords;
using ScreenSize=Size;

#endif //CATAN_VIEWTYPES_HPP