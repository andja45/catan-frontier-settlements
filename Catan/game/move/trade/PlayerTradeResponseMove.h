//
// Created by andja on 16.1.26..
//

#ifndef CATAN_PLAYERTRADERESPONSEMOVE_H
#define CATAN_PLAYERTRADERESPONSEMOVE_H
#include "types/TypeAliases.h"
#include "../Move.h"

class PlayerTradeResponseMove final : public Move {
private:
    TradeId m_tradeRequestId; // we need to know which trade he responded to, in case of more active trades
public:
    PlayerTradeResponseMove(PlayerId respondPlayerId,TradeId tradeRequestId) : Move(respondPlayerId), m_tradeRequestId(tradeRequestId){}

    MoveType type() const override { return MoveType::PlayerTradeResponse; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
    // TODO mark yes as gray if !isvalid in gui (dont need allvalid)
};

#endif //CATAN_PLAYERTRADERESPONSEMOVE_H