//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/EndTurnMove.h"


bool EndTurnMove::isValid(const GameModel& model, const GameSession& session) const {
    return session.isPlayersTurn(m_playerId) && session.canEndTurn();
}

void EndTurnMove::apply(GameModel&, GameSession& session) const {
    // GameSession upravlja fazama i prelazom na sledećeg igraca
    session.endTurn();
}