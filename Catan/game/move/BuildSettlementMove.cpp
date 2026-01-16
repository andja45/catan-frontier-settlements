//
// Created by andja on 10.12.25..
//

#include "BuildSettlementMove.h"
#include "gamemodel/GameSession.h"

bool BuildSettlementMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId)
        return false;

    if (!player.hasSettlementLeft())
        return false;

    if (!player.hasResources(Costs::Settlement))
        return false;

    if (!board.isNodeFree(m_nodeId))
        return false;

    if (board.nodeTouchesAnyBuilding(m_nodeId))
        return false;

    bool connected = false;
    if (session.phase() == TurnPhase::Main) {
        if (!player.hasResources(Costs::Road))
            return false;

        connected =
            board.nodeTouchesPlayerRoad(m_playerId, m_nodeId);
    }
    else if (session.phase() == TurnPhase::InitialPlacement &&
        (session.lastMoveType() == MoveType::InvalidMoveType || // for first move of the game
         session.lastMoveType() == MoveType::BuildRoad)) { // regular flow
        return true; // in initial phase settlements can be placed anywhere, dont need to be connected to roads
        }

    if (!connected)
        return false;

    return true;
}

void BuildSettlementMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    if (session.phase() == TurnPhase::Main) {
        player.removeResources(Costs::Settlement);
    }

    player.addPoints(1);

    Node* node = session.board().getNodeById(m_nodeId);
    player.addSettlement(node); // adds to list of that players houses and decrements numofsettlements left
    board.placeSettlement(m_playerId, m_nodeId);
}

std::unordered_set<EdgeId> BuildSettlementMove::allValid(const GameSession &session) const {
    std::unordered_set<NodeId> validNodes;

    const Board& board = session.board();
    for (NodeId nodeId : board.nodeIds()) {
        BuildSettlementMove probe(*this);
        probe.m_nodeId = nodeId;

        if (probe.isValid(session)) {
            validNodes.insert(nodeId);
        }
    }

    return validNodes;
}