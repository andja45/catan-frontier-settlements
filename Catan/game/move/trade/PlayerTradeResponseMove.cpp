//
// Created by andja on 16.1.26..
//

#include "PlayerTradeResponseMove.h"
#include "model/GameSession.h"
#include "model/Trade.h"

bool PlayerTradeResponseMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);
    const Trade* trade = session.getTrade(m_tradeRequestId);

    // note that it doesnt and it shouldnt be his turn
    if (session.phase() != TurnPhase::Main)
        return false;

    if (!trade)
        return false;

    if (!trade->canRespond(m_playerId)) // if hasnt already responded
        return false;

    if (!player.hasResources(trade->receive()))
        return false;

    return true;
}

void PlayerTradeResponseMove::apply(GameSession& session) const {
    Trade* trade = session.getTrade(m_tradeRequestId);
    if (!trade)
        return;

    trade->markResponded(m_playerId);
}
