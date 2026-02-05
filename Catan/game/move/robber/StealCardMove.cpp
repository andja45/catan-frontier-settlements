//
// Created by andja on 16.1.26..
//

#include "StealCardMove.h"
#include "model/GameSession.h"

bool StealCardMove::isValid(const GameSession& session) const {
    const Board& board = session.board();

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::StealCard)
        return false;

    if (m_playerId == m_victimPlayerId)
        return false;

    if (m_victimPlayerId == types::InvalidPlayerId) // is playerid valid
        return false;

    const Player& victim = session.player(m_victimPlayerId);
    if (victim.getNumOfResourceCards() == 0) // victim must have at least one card
        return false;

    if (!board.tileTouchesPlayerBuilding(m_victimPlayerId, board.robberTile())) // TODO i need tileId where robber is on...
        return false;

    return true;
}

void StealCardMove::apply(GameSession& session) const {
    Player& thief  = session.player(m_playerId);
    Player& victim = session.player(m_victimPlayerId);

    if (victim.getNumOfResourceCards() == 0)
        return; // safety, should not happen if isValid passed

    ResourceType stolen = victim.takeRandomResource();
    thief.addResource(stolen, 1);
}

std::unordered_set<PlayerId> StealCardMove::allValid(const GameSession &session) const {
    std::unordered_set<PlayerId> validPlayers;

    for (PlayerId playerId : session.playerIds()) {
        if (playerId == m_playerId) // for readability
            continue;

        StealCardMove testMove(*this);
        testMove.m_victimPlayerId = playerId;

        if (testMove.isValid(session))
            validPlayers.insert(playerId);
    }

    return validPlayers;
}

