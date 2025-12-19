//
// Created by andja on 17.12.25..
//

#ifndef CATAN_SETTINGROBBERMOVE_H
#define CATAN_SETTINGROBBERMOVE_H

#include "Move.h"


class SettingRobberMove : public Move {
private:
    int m_playerId; // igrac koji je bacio 7
    int m_tileId; // tile koji blokira
    int m_victimPlayerId; // od koga krade karte
public:
    SettingRobberMove(int playerId, int tileId, int victimPlayerId) : m_playerId(playerId), m_tileId(tileId), m_victimPlayerId(victimPlayerId) {}

    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //CATAN_SETTINGROBBERMOVE_H