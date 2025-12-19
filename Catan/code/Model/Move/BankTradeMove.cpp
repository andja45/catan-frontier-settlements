//
// Created by andja on 17.12.25..
//

#include "../../../headers/Move/BankTradeMove.h"

bool BankTradeMove::isValid(const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canTrade()) return false;

    GameModel& model = session.model();
    int ratio = model.tradeRatioFor(m_playerId, m_give);
    return model.hasResource(m_playerId, m_give, ratio);
}

void BankTradeMove::apply(GameSession& session) const {
    GameModel& model = session.model();
    int ratio = model.tradeRatioFor(m_playerId, m_give);

    model.consumeResource(m_playerId, m_give, ratio); // da banci
    model.transferResource(m_playerId, m_receive, 1); // dobije od banke
}