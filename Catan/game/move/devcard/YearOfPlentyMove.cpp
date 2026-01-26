//
// Created by andja on 16.1.26.
//

#include "YearOfPlentyMove.h"
#include "model/GameSession.h"

bool YearOfPlentyMove::isValid(const GameSession& session) const {
    const Bank& bank = session.bank();

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::YearOfPlenty)
        return false;

    if (m_first == ResourceType::None || m_second == ResourceType::None)
        return false;

    // we have to check can bank supply the player
    if (!bank.hasResource(m_first, 1))
        return false;

    if (m_second == m_first) {
        if (!bank.hasResource(m_first, 2))
            return false;
    } else {
        if (!bank.hasResource(m_second, 1))
            return false;
    }

    return true;
}

void YearOfPlentyMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);
    Bank& bank = session.bank();

    ResourcePack gained;
    gained[m_first] += 1;
    gained[m_second] += 1;

    bank.removeResources(gained);
    player.addResources(gained);
}

