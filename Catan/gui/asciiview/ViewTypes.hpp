//
// Created by matija on 12/21/25.
//

#ifndef CATAN_VIEWTYPES_HPP
#define CATAN_VIEWTYPES_HPP

#include <utility>
#include <vector>
#include <cstdint>

struct BoardTheme {
    char tileChar = ' ';
    char upDownEdgeChar = '-';
    char upLeftDownRightEdgeChar = '/';
    char upRightDownLeftEdgeChar = '\\';

    char woolChar='w';
    char woodChar='T';
    char brickChar='=';
    char oreChar='o';
    char wheatChar='i';
    char seaChar='~';
    char desertChar=',';

    char settlementChar='*';
    char cityChar='@';

    char robberChar='X';

    uint8_t playerColor[5]{};
    uint8_t outlineColor=1;
};

struct Cell { char ch; std::uint8_t color; };
using Canvas=std::vector<std::vector<Cell>>;
using ScreenCoords=std::pair<int,int>;

#endif //CATAN_VIEWTYPES_HPP