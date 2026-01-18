//
// Created by andja on 10.12.25..
//

#ifndef BUILDCITYMOVE_H
#define BUILDCITYMOVE_H

#include <unordered_set>

#include "../Move.h"


class BuildCityMove final : public Move{
private:
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildCityMove(const PlayerId playerId, const NodeId nodeId) : Move(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildCity; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<NodeId> allValid(const GameSession &session) const;
};


#endif //BUILDCITYMOVE_H