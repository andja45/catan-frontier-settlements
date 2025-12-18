//
// Created by andja on 10.12.25..
//

#ifndef ENDTURNMOVE_H
#define ENDTURNMOVE_H

#include "Move.h"


class EndTurnMove : public Move{
private:
    int m_playerId;
public:
    explicit EndTurnMove(int playerId) : m_playerId(playerId) {}

    bool isValid(const GameModel&, const GameSession&) const override;
    void apply(GameModel&, GameSession&) const override;
};


#endif //ENDTURNMOVE_H