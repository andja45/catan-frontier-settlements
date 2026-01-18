//
// Created by andja on 16.1.26.
//

#include "MonopolyMove.h"
#include "gamemodel/GameSession.h"

bool MonopolyMove::isValid(const GameSession& session) const {
    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Monopoly)
        return false;

    if (m_resource == ResourceType::None) // even tho ui doesnt let this, we still check
        return false;

    return true;
}

void MonopolyMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);

    ResourcePack collected;
    for (PlayerId pid: session.playerIds()) {
        if (pid == m_playerId)
            continue;

        Player& otherPlayer = session.player(pid);
        int amount = otherPlayer.getNumOfResourceCards(m_resource);
        if (amount <= 0)
            continue;

        otherPlayer.removeResource(m_resource, amount);
        collected[m_resource] += amount;
    }

    if (!collected.empty()) {
        player.addResources(collected);
    }
}

