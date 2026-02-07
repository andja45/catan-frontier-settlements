//
// Created by andja on 16.1.26.
//

#include "PlayerTradeRequestMove.h"
#include "model/GameSession.h"
#include "model/Trade.h"

bool PlayerTradeRequestMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);
    if (m_tradeId==-1)
        return false;
    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (m_give.empty() || m_receive.empty()) // cannot send/request empty trade
        return false;

    if (!player.hasResources(m_give))
        return false;

    return true;
}

void PlayerTradeRequestMove::apply(GameSession& session) const {
    Trade trade(m_playerId, m_give, m_receive,m_tradeId); // creates trade
    session.addTrade(std::move(trade)); // adds trade to activeTrades
}

