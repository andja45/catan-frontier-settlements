//
// Created by andja on 10.12.25..
//

#ifndef BUILDROADMOVE_H
#define BUILDROADMOVE_H

#include <unordered_set>

#include "move/BoardMove.h"

class BuildRoadMove final : public BoardMove{ // final - no classes than extend this class
private:
    EdgeId m_edgeId; // or HexCoords + edgeIndex
public:
    BuildRoadMove(PlayerId playerId, EdgeId edgeId) : BoardMove(playerId), m_edgeId(edgeId) {}

    MoveType type() const override { return MoveType::BuildRoad; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<EdgeId> allValid(const GameSession& session) const;

    EdgeId getEdgeId() const { return m_edgeId; }
    void setBoardElementId(int elementId) { m_edgeId = static_cast<EdgeId>(elementId); }
};


#endif //BUILDROADMOVE_H