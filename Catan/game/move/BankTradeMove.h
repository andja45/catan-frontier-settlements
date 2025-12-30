//
// Created by andja on 17.12.25..
//

#ifndef CATAN_BANKTRADEMOVE_H
#define CATAN_BANKTRADEMOVE_H

#include "Move.h"


class BankTradeMove : public Move{
private:
    int m_playerId;
    ResourceType m_give;
    ResourceType m_receive;
public:
    explicit BankTradeMove(int playerId, ResourceType give, ResourceType receive) : m_playerId(playerId), m_give(give), m_receive(receive) {}

    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //CATAN_BANKTRADEMOVE_H