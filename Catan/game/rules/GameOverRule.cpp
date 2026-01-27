//
// Created by andja on 6.1.26..
//

#include "GameOverRule.h"
#include "model/GameSession.h"

void GameOverRule::evaluate(GameSession& session) {
    for (const auto& p : session.players()) {
        if (p->getTotalPoints() >= session.winningPoints()) {
            session.setWinner(p->getPlayerId());
            session.endGame();
            return;
        }
    }

    if (session.activePlayersLeft()<=0)
        session.endGame();
}