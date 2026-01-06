//
// Created by andja on 11.12.25.
//

#include "GameSession.h"

#include <cassert>
#include <move/Move.h>

InitialPlacementStep GameSession::initialPlacementStep() const {
    return (m_initialPlacementsCount % 2 == 0)
        ? InitialPlacementStep::PlaceSettlement
        : InitialPlacementStep::PlaceRoad;
}

GameSession::GameSession(int numPlayers,
                         std::vector<std::string> playerNames,
                         PlayerId localPlayer,
                         uint32_t seed)
    : m_board(std::make_unique<Board>())
    , m_localPlayerId(localPlayer)
    , m_rng(seed)
{
    m_players.reserve(numPlayers);

    for (PlayerId id = 0; id < numPlayers; ++id) {
        m_players.push_back(std::make_unique<Player>(id, playerNames[id]));
    }
}

bool GameSession::applyMove(const Move& move){
    if (!move.isValid(*this))
        return false;

    move.apply(*this);
    m_rules.evaluate(*this); // after every move, cus someone can interrupt
    advancePhaseAfterMove(move); // only session can advance phases, move only reads them
    //notifyModelChanged(); //notify view

    return true;
}

void GameSession::advancePhaseAfterMove(const Move& move) {
    if (m_phase == TurnPhase::InitialPlacement) {
        advanceInitialPlacement();
        return;
    }

    switch (move.type()) {
        case MoveType::RollDice:
            setPhase(TurnPhase::Main);
            break;

        case MoveType::EndTurn:
            advancePlayer();
            setPhase(TurnPhase::RollDice);
            break;

        default:
            break;
    }
}

void GameSession::advanceInitialPlacement() {
    const int playerCount = numPlayers();
    if (playerCount == 0) return;

    m_initialPlacementsCount += 1;

    const int totalPlacementMoves = playerCount * 4; // *2 for both directios *2 for settlemet/road

    if (m_initialPlacementsCount >= totalPlacementMoves) {
        m_initialPlacementsCount = 0;
        m_turnIndex = 0;
        m_currentPlayerId = m_players[m_turnIndex]->getPlayerId(); // first player starts
        setPhase(TurnPhase::RollDice);
        return;
    }

    const int placementTurn = m_initialPlacementsCount / 2; // each player gets settlement + road in one placing

    int playerIndex;
    if (placementTurn < playerCount) {
        playerIndex = placementTurn;
    }
    else {
        playerIndex = (playerCount - 1) - (placementTurn - playerCount);
    }

    m_currentPlayerId = m_players[playerIndex]->getPlayerId();
}


void GameSession::advancePlayer() { // TODO maybe change later - add shuffling with same seed every client model uses
    if (m_players.empty()) return;

    m_turnIndex = (m_turnIndex + 1) % m_players.size();
    m_currentPlayerId = m_players[m_turnIndex]->getPlayerId();
}

int GameSession::rollDice() {
    assert(m_phase == TurnPhase::RollDice);

    int dice1 = m_d6(m_rng);
    int dice2 = m_d6(m_rng);
    return dice1 + dice2;
}