//
// Created by andja on 10.12.25..
//

#ifndef ROLLDICEMOVE_H
#define ROLLDICEMOVE_H

#include "../Move.h"


class RollDiceMove final : public Move{ // TODO consider adding dice rolled number that is set on server side (or client for convenience if we dont care about cheating)
public:
    explicit RollDiceMove(PlayerId playerId) : Move(playerId){}
    // RollDiceMove move = 3; jer ne zelimo da moze to, samo je generisanje dozvoljeno

    MoveType type() const override { return MoveType::RollDice; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ROLLDICEMOVE_H