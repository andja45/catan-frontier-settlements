//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/RollDiceMove.h"

bool RollDiceMove::isValid(const GameModel& model, const GameSession& session) const {
    return session.isPlayersTurn(m_playerId) && session.canRollDice();
}

void RollDiceMove::apply(GameModel& model, GameSession&) const {
    int dice = model.rollDice(); // necemo svaki put generisati random seed, ima jedna kockica i drzi je model

    if (dice == 7) {
        session.enterRobberPhase();
    } else {
        model.distributeResources(dice);
    }
}