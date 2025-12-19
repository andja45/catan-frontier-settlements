//
// Created by andja on 11.12.25..
//

#include "../../../headers/Game/GameSession.h"

bool GameSession::applyMove(const Move& move){
    if (!move.isValid(*this)) {
        return false;
    }

    move.apply(*this);

    // updatePhaseAfterMove(move);

    return true;
}

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

bool GameSession::canPlaceRobber() const {
    return m_phase == TurnPhase::Robber;
}

bool GameSession::canEndTurn() const {
    return m_phase == TurnPhase::Main;
}

// --- Transitions ---
void GameSession::enterRobberPhase() {
    m_phase = TurnPhase::Robber;
}

void GameSession::enterMainPhase() {
    m_phase = TurnPhase::Main;
}

void GameSession::endTurn() {
    // TODO
    // dodaj prelaz na sl igraca, ako je u initalplacement ostaje tu,
    // inace ide u rolldice - ali nije tako jednostavno, treba impl i izlazak
    // iz initialplacement(svi se postavili)
}
