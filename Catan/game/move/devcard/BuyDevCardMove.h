//
// Created by andja on 16.1.26..
//

#ifndef CATAN_BUYDEVCARD_H
#define CATAN_BUYDEVCARD_H

#include "move/Move.h"

class BuyDevCardMove final : public Move {
public:
    explicit BuyDevCardMove(PlayerId playerId) : Move(playerId) {}

    MoveType type() const override { return MoveType::BuyDevCard; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
};


#endif //CATAN_BUYDEVCARD_H