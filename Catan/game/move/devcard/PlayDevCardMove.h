//
// Created by andja on 16.1.26..
//

#ifndef CATAN_ACTIVEDEVCARDMOVE_H
#define CATAN_ACTIVEDEVCARDMOVE_H

#include "move/Move.h"
#include "types/DevCardType.h"

class PlayDevCardMove final : public Move {
private:
    DevCardType m_card;
public:
    PlayDevCardMove(PlayerId playerId, DevCardType card) : Move(playerId), m_card(card) {}

    MoveType type() const override { return MoveType::PlayDevCard; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    DevCardType getCardType() const {return m_card;};

};


#endif //CATAN_ACTIVEDEVCARDMOVE_H