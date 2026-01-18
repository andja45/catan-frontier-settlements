//
// Created by andja on 10.12.25..
//

#include "BuildCityMove.h"
#include "gamemodel/GameSession.h"


bool BuildCityMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId) // keep in mind this is not true for every move some are played by all players (accept trade, discard cards)
        return false;

    if (session.phase() != TurnPhase::Main) // only happens in main
        return false;

    if (!player.hasCityLeft())
        return false;

    if (!player.hasResources(Costs::Settlement))
        return false;

    if (!board.isNodeSettlement(m_nodeId))
        return false;

    if (!board.isBuildingOwnedBy(m_playerId, m_nodeId))
        return false;

    return true;
}

void BuildCityMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    player.removeResources(Costs::City);

    player.addPoints(1);

    board.placeCity(m_playerId, m_nodeId);
}

std::unordered_set<NodeId> BuildCityMove::allValid(const GameSession &session) const {
    std::unordered_set<NodeId> validNodes;

    const Board& board = session.board();
    for (NodeId nodeId : board.nodeIds()) {
        BuildCityMove testMove(*this);
        testMove.m_nodeId = nodeId;

        if (testMove.isValid(session)) {
            validNodes.insert(nodeId);
        }
    }

    return validNodes;
}