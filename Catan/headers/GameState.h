//
// Created by andja on 10.12.25..
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

#include "Board.h"

class Player;

/* kao skrinsot tr stanja igre */

class GameState {
private:
    Board _board;
    std::vector<Player*> _players;
public:

};


#endif //GAMESTATE_H