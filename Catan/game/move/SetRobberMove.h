//
// Created by andja on 17.12.25.
//

#ifndef CATAN_SETTINGROBBERMOVE_H
#define CATAN_SETTINGROBBERMOVE_H

#include "Move.h"


class SetRobberMove final : public Move {
private:
    PlayerId m_playerId; // igrac koji je bacio 7
    TileId m_tileId; // tile koji blokira
    PlayerId m_victimPlayerId; // od koga krade karte
public:
    SetRobberMove(PlayerId playerId, TileId tileId, PlayerId victimPlayerId) : m_playerId(playerId), m_tileId(tileId), m_victimPlayerId(victimPlayerId) {}

    MoveType type() const override { return MoveType::SetRobber; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //CATAN_SETTINGROBBERMOVE_H