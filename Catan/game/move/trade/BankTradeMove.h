//
// Created by andja on 17.12.25.
//

#ifndef CATAN_BANKTRADEMOVE_H
#define CATAN_BANKTRADEMOVE_H

#include "../Move.h"

class BankTradeMove final : public Move{
private:
    ResourceType m_give;
    ResourceType m_receive;
public:
    explicit BankTradeMove(PlayerId playerId, ResourceType give, ResourceType receive)
        : Move(playerId), m_give(give), m_receive(receive) {}

    MoveType type() const override { return MoveType::BankTrade; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;

    ResourceType getGive() const {return m_give;}
    ResourceType getReceive() const {return m_receive;}

    // TODO gui takes care if >= mintrade then unclickable (mintrade is written under player rss and we get it from player)
};


#endif //CATAN_BANKTRADEMOVE_H