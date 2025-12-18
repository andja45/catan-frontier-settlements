//
// Created by andja on 17.12.25..
//

#include "../../../headers/Move/BankTradeMove.h"

bool BankTradeMove::isValid(const GameModel& model, const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canTrade()) return false;

    int ratio = model.tradeRatioFor(m_playerId, m_give);
    return model.hasResource(m_playerId, m_give, ratio);
}

void BankTradeMove::apply(GameModel& model, GameSession&) const {
    int ratio = model.tradeRatioFor(m_playerId, m_give);

    model.consumeResource(m_playerId, m_give, ratio);
    model.addResource(m_playerId, m_receive, 1);
}