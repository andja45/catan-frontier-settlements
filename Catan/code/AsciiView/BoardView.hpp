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

enum class ResourceType;
class Tile;
class Board;

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

    uint8_t playerColor[5];
    uint8_t outlineColor;
};

struct Cell { char ch; std::uint8_t color; };
using Canvas=std::vector<std::vector<Cell>>;
using ScreenCoords=std::pair<int,int>;
//res to char, player col

class BoardView {
private:

    static ScreenCoords constexpr sizeBig={18,7};
    static ScreenCoords constexpr sizeSmall={12,5};

    Board* m_board;
    BoardTheme m_theme;

    Canvas m_cells;

    ScreenCoords m_size;
    ScreenCoords m_center;

    //std::map<int,std::pair<int,int>> m_computedTilePositions;
    //void drawTile(Tile* tile, std::pair<int,int> pos);

    void init();

    uint8_t playerColor(int playerId);
    char directionToChar(SideDirection dir);

    ScreenCoords axialToScreen(HexCoords axial);

    void printChar(ScreenCoords, char c, uint8_t color=1);

    void renderBoard();
    void blitBoard();
public:
    void drawBoard();
};

#endif //CATAN_BOARDVIEW_HPP