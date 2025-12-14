//
// Created by andja on 10.12.25..
//

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
class Tile;


class Board {
private:
    //dodaj vectore
    //dodaj koordinate u sve?
    std::map<int, std::vector<Tile*>> _tilesByNumber;
public:
    std::map<int, std::vector<Tile*>> getTilesByNumber() { return _tilesByNumber; };
};


#endif //BOARD_H