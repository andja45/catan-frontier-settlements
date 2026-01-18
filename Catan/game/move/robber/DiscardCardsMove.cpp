//
// Created by andja on 16.1.26.
//

#include "DiscardCardsMove.h"
#include  "model/GameSession.h"

bool DiscardCardsMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);

    if (session.phase() != TurnPhase::DiscardCards)
        return false;

    if (!session.playerMustDiscard(m_playerId))
        return false;

    if (session.hasPlayerDiscarded(m_playerId)) // if player has 50 cards he discards 25, we dont want him to discard again, he would pass >= 7 check
        return false;

    const int playerTotalCards = player.getNumOfResourceCards();
    const int required = playerTotalCards / 2;

    int totalCardsDiscarded = 0;
    for (const auto& [type, amount] : m_discarded) {
        totalCardsDiscarded += amount;
    }

    if (totalCardsDiscarded != required)
        return false;

    if (!player.hasResources(m_discarded))
        return false;

    return true;
}

void DiscardCardsMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);

    player.removeResources(m_discarded);
    session.markPlayerDiscarded(m_playerId);
}
