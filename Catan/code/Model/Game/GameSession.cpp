//
// Created by andja on 11.12.25..
//

#include "../../../headers/Game/GameSession.h"

// --- Guards (koristi ih Move::isValid) ---
bool GameSession::canRollDice() const {
    return m_phase == TurnPhase::RollDice;
}

bool GameSession::canBuild() const {
    return m_phase == TurnPhase::Main
        || m_phase == TurnPhase::InitialPlacement;
}

bool GameSession::canTrade() const {
    return m_phase == TurnPhase::Main;
}

bool GameSession::canEndTurn() const {
    return m_phase == TurnPhase::Main;
}

// --- Transitions ---
void GameSession::enterRobberPhase() {
    m_phase = TurnPhase::Robber;
    // notifyObservers();
}

void GameSession::endTurn() { // TODO dodaj prelaz na sl igraca
    m_phase = TurnPhase::End;
    // notifyObservers();
}
