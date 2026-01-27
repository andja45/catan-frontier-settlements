//
// Created by matija on 1/27/26.
//

#include "PlayerLeave.hpp"

//
// Created by andja on 10.12.25..
//

#include "EndTurnMove.h"
#include "model/GameSession.h"


bool PlayerLeave::isValid(const GameSession &) const { //server checks!
    return true;
}

void PlayerLeave::apply(GameSession & session) const {
    session.leavePlayer(m_playerId);
}
