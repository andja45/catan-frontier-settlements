//
// Created by andja on 6.1.26..
//

#include "VictoryRule.h"
#include "gamemodel/GameSession.h"

void VictoryRule::evaluate(GameSession& session) {
    for (const auto& p : session.players()) {
        if (p->getTotalPoints() >= session.winningPoints()) {
            session.setWinner(p->getPlayerId());
            session.endGame();
            return;
        }
    }
}