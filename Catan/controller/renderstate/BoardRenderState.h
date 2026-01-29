//
// Created by andja on 16.1.26..
//

#ifndef CATAN_BOARDRENDERSTATE_H
#define CATAN_BOARDRENDERSTATE_H

#include <unordered_set>

#include "RenderState.h"
#include "move/MoveType.h"
#include "types/TypeAliases.h"


class BoardRenderState final : public RenderState {
private:
    std::unordered_set<NodeId> m_highlightedNodes;
    std::unordered_set<EdgeId> m_highlightedEdges;
    std::unordered_set<TileId> m_highlightedTiles;
public:
    void setHighlighted(std::unordered_set<int> ids, MoveType type);

    bool isNodeHighlighted(NodeId nodeId) const;
    bool isEdgeHighlighted(EdgeId edgeId) const;
    bool isTileHighlighted(TileId tileId) const;

    void clear(); // called after endmove applied (everything disabled until renderstate updated again)
};


#endif //CATAN_BOARDRENDERSTATE_H