//
// Created by andja on 17.12.25.
//

#include "BankTradeMove.h"
#include "model/GameSession.h"
#include "player/Bank.h"

bool BankTradeMove::isValid(const GameSession& session) const{
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (m_give == m_receive) // cant trade for same rss
        return false;

    const int ratio = player.minBankTradeRatio(m_give);

    return player.hasResource(m_give, ratio); // player always gives minratio
}

void BankTradeMove::apply(GameSession& session) const{
    Player& player = session.player(m_playerId);
    Bank& bank = session.bank();

    const int ratio = player.minBankTradeRatio(m_give);

    // Pay bank
    player.removeResource(m_give, ratio);
    bank.addResource(m_give, ratio);

    // Receive from bank
    bank.removeResource(m_receive, 1);
    player.addResource(m_receive, 1);
}