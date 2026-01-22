//
// Created by andja on 16.1.26..
//

#include "BuyDevCardMove.h"
#include "model/GameSession.h"

bool BuyDevCardMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);
    const Bank& bank = session.bank();

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (!bank.hasDevCards()) // checks does bank have any devcards left
        return false;

    if (!player.hasResources(Costs::DevCard))
        return false;

    return true;
}

void BuyDevCardMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);
    Bank& bank = session.bank();

    player.removeResources(Costs::DevCard);

    DevCardType card = bank.takeRandomDev();
    player.addDevCard(card);
}

