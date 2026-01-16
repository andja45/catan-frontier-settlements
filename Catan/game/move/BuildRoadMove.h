//
// Created by andja on 10.12.25..
//

#ifndef BUILDROADMOVE_H
#define BUILDROADMOVE_H

#include <unordered_set>

#include "Move.h"


class BuildRoadMove final : public Move{ // final - no classes than extend this class
private:
    PlayerId m_playerId;
    EdgeId m_edgeId; // or HexCoords + edgeIndex
public:
    BuildRoadMove(PlayerId playerId, EdgeId edgeId) : m_playerId(playerId), m_edgeId(edgeId) {}

    MoveType type() const override { return MoveType::BuildRoad; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<EdgeId> allValid(const GameSession& session) const;
};


#endif //BUILDROADMOVE_H