//
// Created by andja on 10.12.25..
//

#ifndef ROLLDICEMOVE_H
#define ROLLDICEMOVE_H

#include "Move.h"


class RollDiceMove final : public Move{
private:
    PlayerId m_playerId;
public:
    explicit RollDiceMove(PlayerId playerId) : m_playerId(playerId){}
    // RollDiceMove move = 3; jer ne zelimo da moze to, samo je generisanje dozvoljeno

    MoveType type() const override { return MoveType::RollDice; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ROLLDICEMOVE_H