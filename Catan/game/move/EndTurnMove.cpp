//
// Created by andja on 10.12.25..
//

#include "EndTurnMove.h"


bool EndTurnMove::isValid(const GameSession& session) const {
    return session.isPlayersTurn(m_playerId) && session.canEndTurn();
}

void EndTurnMove::apply(GameSession& session) const {
    // GameSession upravlja fazama i prelazom na sledeceg igraca
    session.endTurn();
}