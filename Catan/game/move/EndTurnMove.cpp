//
// Created by andja on 10.12.25..
//

#include "EndTurnMove.h"
#include "gamemodel/GameSession.h"


bool EndTurnMove::isValid(const GameSession& session) const {
    if (session.currentPlayer() != m_playerId) // TODO game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves
        return false;

    if (session.phase() != TurnPhase::Main) // this is fine because activating robber will enter robberphase so you cant end move
        return false;

    return true;
}

void EndTurnMove::apply(GameSession& session) const {
    // GameSession changes phases and advances players
}