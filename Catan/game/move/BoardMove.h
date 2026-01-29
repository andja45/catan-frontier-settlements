//
// Created by andja on 29.1.26..
//

#ifndef BOARDMOVE_H
#define BOARDMOVE_H

#include "Move.h"

class BoardMove : public Move {
public:
    using Move::Move;
    virtual void setBoardElementId(int id) = 0;
};

#endif //BOARDMOVE_H