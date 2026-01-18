//
// Created by andja on 16.1.26..
//

#ifndef CATAN_PLAYERTRADEREQUEST_H
#define CATAN_PLAYERTRADEREQUEST_H

#include "types/TypeAliases.h"
#include "../Move.h"

class PlayerTradeRequestMove final : public Move {
private:
    ResourcePack m_give;
    ResourcePack m_receive;
public:
    PlayerTradeRequestMove(PlayerId requestPlayerId, ResourcePack giveResources,ResourcePack receiveResources) : Move(requestPlayerId), m_give(giveResources), m_receive(receiveResources){}

    MoveType type() const override { return MoveType::PlayerTradeRequest; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
};
#endif //CATAN_PLAYERTRADEREQUEST_H