//
// Created by andja on 17.1.26..
//

#ifndef CATAN_PLAYERTRADEACCEPTMOVE_H
#define CATAN_PLAYERTRADEACCEPTMOVE_H

#include <unordered_set>
#include "../Move.h"

class PlayerTradeAcceptMove final : public Move {
private:
    PlayerId m_acceptedPlayerId;
    TradeId m_tradeId;
public:
    PlayerTradeAcceptMove(PlayerId playerId, PlayerId acceptedPlayerId, TradeId tradeId)
        : Move(playerId), m_acceptedPlayerId(acceptedPlayerId), m_tradeId(tradeId) {}

    MoveType type() const override { return MoveType::PlayerTradeAccept; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<PlayerId> allValid(const GameSession &session) const; // players that he can chose from
    PlayerId getAcceptedPlayerId() const {return m_acceptedPlayerId;}
    TradeId getTradeId() const {return m_tradeId;}
};


#endif //CATAN_PLAYERTRADEACCEPTMOVE_H