//
// Created by andja on 17.12.25.
//

#ifndef CATAN_BANKTRADEMOVE_H
#define CATAN_BANKTRADEMOVE_H

#include "Move.h"


class BankTradeMove final : public Move{
private:
    PlayerId m_playerId;
    ResourceType m_give;
    ResourceType m_receive;
public:
    explicit BankTradeMove(PlayerId playerId, ResourceType give, ResourceType receive) : m_playerId(playerId), m_give(give), m_receive(receive) {}

    MoveType type() const override { return MoveType::BankTrade; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //CATAN_BANKTRADEMOVE_H