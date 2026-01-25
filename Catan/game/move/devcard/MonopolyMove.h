//
// Created by andja on 16.1.26..
//

#ifndef CATAN_MONOPOLYMOVE_H
#define CATAN_MONOPOLYMOVE_H

#include "move/Move.h"

class MonopolyMove final : public Move {
private:
    ResourceType m_resource;
public:
    MonopolyMove(PlayerId playerId, ResourceType resource) : Move(playerId), m_resource(resource) {}

    MoveType type() const override { return MoveType::Monopoly; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    ResourceType getResource() const {return m_resource;}
};


#endif //CATAN_MONOPOLYMOVE_H