//
// Created by andja on 17.12.25.
//

#ifndef CATAN_SETTINGROBBERMOVE_H
#define CATAN_SETTINGROBBERMOVE_H

#include <unordered_set>
#include "../Move.h"


class SetRobberMove final : public Move {
private:
    TileId m_tileId;
public:
    SetRobberMove(PlayerId playerId, TileId tileId) : Move(playerId), m_tileId(tileId) {}

    MoveType type() const override { return MoveType::SetRobber; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<TileId> allValid(const GameSession& session) const;
};


#endif //CATAN_SETTINGROBBERMOVE_H