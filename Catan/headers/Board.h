//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include "Tile.h"

class Board {
private:
    //dodaj vectore
    //dodaj koordinate u sve?
    std::map<int, std::vector<Tile*>> m_tilesByNumber;
    std::vector<Tile*> m_tiles;
public:
    Board() { randomBoard(); }
    std::map<int, std::vector<Tile*>> getTilesByNumber() { return m_tilesByNumber; };
    void randomBoard();
};


#endif //BOARD_H
