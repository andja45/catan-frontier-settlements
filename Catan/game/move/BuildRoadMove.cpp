//
// Created by andja on 10.12.25.
//

#include "BuildRoadMove.h"
#include "gamemodel/GameSession.h"

bool BuildRoadMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId)
        return false;

    if (!player.hasRoadLeft())
        return false;

    if (!board.isEdgeFree(m_edgeId))
        return false;

    // connected check, (depends on phase)
    bool connected = false;
    if (session.phase() == TurnPhase::Main) {  // road needs to be connected to player's road or building
        if (!player.hasResources(Costs::Road))
            return false;

        connected = board.edgeTouchesPlayersRoad(m_playerId, m_edgeId);
    }
    else if (session.phase() == TurnPhase::InitialPlacement &&
        session.lastMoveType() == MoveType::BuildSettlement){ // or both phases in separate check at start and this just in else
        if (!player.hasHouses())
            return false;

        Node* lastSettlement = player.getLastHouseBuilt(); // TODO or just function that remembers lastbuiltSettlement, if we do computeLongestRoad in board (update will be in session)
        connected = board.edgeTouchesNode(lastSettlement->getNodeId(), m_edgeId);  // road need to touch last placed house
    }

    if (!connected)
        return false;

    return true;
}

void BuildRoadMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    if (session.phase() == TurnPhase::Main) {
        player.removeResources(Costs::Road);
    }

    Edge* edge= session.board().getEdgeById(m_edgeId);
    player.addRoad(edge); // adds to list of that players edges and decrements numofroads left
    board.placeRoad(m_playerId, m_edgeId);
}

std::unordered_set<EdgeId> BuildRoadMove::allValid(const GameSession &session) const {
    std::unordered_set<EdgeId> validEdges;

    const Board& board = session.board();
    for (EdgeId edgeId : board.edgeIds()) {
        BuildRoadMove probe(*this);
        probe.m_edgeId = edgeId;

        if (probe.isValid(session)) {
            validEdges.insert(edgeId);
        }
    }

    return validEdges;
}
