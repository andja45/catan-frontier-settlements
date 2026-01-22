//
// Created by andja on 16.1.26..
//

#include "PlayDevCardMove.h"
#include "model/GameSession.h"

bool PlayDevCardMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (!player.hasDevCard(m_card))
        return false;

    if (session.hasPlayedDevCardThisTurn()) // only one devcard per turn!
        return false;

    // TODO maybe add that you cant play devcard you bought this turn

    return true;
}

void PlayDevCardMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);

    player.removeDevCard(m_card);
    session.markDevCardPlayedThisTurn(); // session translates it into equivalent phase

    session.enterDevCardPhase(m_card);
}

