//
// Created by andja on 10.12.25..
//

#include "EndTurnMove.h"
#include "model/GameSession.h"


bool EndTurnMove::isValid(const GameSession& session) const {
    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main) // this is fine because activating robber will enter robberphase so you cant end move
        return false;

    return true;
}

void EndTurnMove::apply(GameSession& session) const {
    //TODO GameSession changes phases and advances players
}