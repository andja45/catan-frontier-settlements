//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/PlayerTradeMove.h"


bool PlayerTradeMove::isValid(const GameSession& session) const {
    if (!session.isPlayersTurn(m_fromPlayer)) return false;
    if (!session.canTrade()) return false;

    const GameModel& model = session.model();
    return model.hasResources(m_fromPlayer, m_offer) &&
           model.hasResources(m_toPlayer, m_request);
}

void PlayerTradeMove::apply(GameSession& session) const {
    GameModel& model = session.model();

    model.transferResources(m_fromPlayer, m_toPlayer, m_offer);
    model.transferResources(m_toPlayer, m_fromPlayer, m_request);
}