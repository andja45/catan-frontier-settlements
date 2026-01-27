//
// Created by matija on 1/27/26.
//

#include "SkipPlayerRule.hpp"
#include "model/GameSession.h"

void SkipPlayerRule::evaluate(GameSession &session) {
    if (!session.player(session.currentPlayer()).isActive()) {
        session.advancePlayer();
    }
}
