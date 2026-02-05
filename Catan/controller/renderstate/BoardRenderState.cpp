//
// Created by andja on 16.1.26..
//

#include "BoardRenderState.h"

#include "move/Move.h"

void BoardRenderState::setHighlighted(std::unordered_set<int> ids, MoveType type) {
    clear();

    switch (type) {
        case MoveType::BuildRoad:
            for (int id : ids) m_highlightedEdges.insert(static_cast<EdgeId>(id));
            break;

        case MoveType::BuildSettlement:
        case MoveType::BuildCity:
            for (int id : ids) m_highlightedNodes.insert(static_cast<NodeId>(id));
            break;

        case MoveType::SetRobber:
            for (int id : ids) m_highlightedTiles.insert(static_cast<TileId>(id));
            break;

        default:
            break;
    }
}

std::vector<EdgeId> BoardRenderState::getHighlightedEdges() const {
    return std::vector(m_highlightedEdges.begin(), m_highlightedEdges.end());
}

std::vector<NodeId> BoardRenderState::getHighlightedNodes() const {
    return std::vector(m_highlightedNodes.begin(), m_highlightedNodes.end());
}

std::vector<TileId> BoardRenderState::getHighlightedTiles() const {
    return std::vector(m_highlightedTiles.begin(), m_highlightedTiles.end());
}

bool BoardRenderState::isNodeHighlighted(NodeId nodeId) const {
    return m_highlightedNodes.find(nodeId) != m_highlightedNodes.end();
}

bool BoardRenderState::isEdgeHighlighted(EdgeId edgeId) const {
    return m_highlightedEdges.find(edgeId) != m_highlightedEdges.end();
}

bool BoardRenderState::isTileHighlighted(TileId tileId) const {
    return m_highlightedTiles.find(tileId) != m_highlightedTiles.end();
}

bool BoardRenderState::isPlayerHighlighted(PlayerId playerId) const {
    return m_highlightedPlayers.find(playerId) != m_highlightedPlayers.end();
}

void BoardRenderState::clear() {
    m_highlightedNodes.clear();
    m_highlightedEdges.clear();
    m_highlightedTiles.clear();
    m_highlightedPlayers.clear(); // difficult in gui, gui shouldnt have to care about that
}
