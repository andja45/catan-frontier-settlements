//
// Created by andja on 16.1.26..
//

#include "BoardRenderState.h"

#include "move/Move.h"

bool BoardRenderState::isNodeHighlighted(NodeId nodeId) const {
    return m_highlightedNodes.find(nodeId) != m_highlightedNodes.end();
}

bool BoardRenderState::isEdgeHighlighted(EdgeId edgeId) const {
    return m_highlightedEdges.find(edgeId) != m_highlightedEdges.end();
}

bool BoardRenderState::isTileHighlighted(TileId tileId) const {
    return m_highlightedTiles.find(tileId) != m_highlightedTiles.end();
}
