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
    std::vector<int> m_numberOrder = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    void randomBoard();
    static std::vector<std::tuple<int,int,int>> generateCoordinates();
public:
    Board() { randomBoard(); }
    std::map<int, std::vector<Tile*>> getTilesByNumber() { return m_tilesByNumber; };
};


#endif //BOARD_H
