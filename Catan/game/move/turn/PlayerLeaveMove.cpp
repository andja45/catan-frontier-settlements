//
// Created by matija on 1/27/26.
//

#include "PlayerLeaveMove.hpp"

//
// Created by andja on 10.12.25..
//

#include "EndTurnMove.h"
#include "model/GameSession.h"


bool PlayerLeaveMove::isValid(const GameSession &) const { //server checks!
    return true;
}

void PlayerLeaveMove::apply(GameSession & session) const {
    session.leavePlayer(m_playerId);
}
