//
// Created by andja on 10.12.25..
//

#ifndef ROLLDICEMOVE_H
#define ROLLDICEMOVE_H

#include "Move.h"


class RollDiceMove : public Move{
private:
    int m_playerId;
public:
    explicit RollDiceMove(int playerId) : m_playerId(playerId){}
    // RollDiceMove move = 3; jer ne zelimo da moze to, samo je generisanje dozvoljeno

    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ROLLDICEMOVE_H