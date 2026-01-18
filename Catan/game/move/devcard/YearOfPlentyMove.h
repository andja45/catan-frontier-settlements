//
// Created by andja on 16.1.26..
//

#ifndef CATAN_YEAROFPLENTYMOVE_H
#define CATAN_YEAROFPLENTYMOVE_H

#include "move/Move.h"

class YearOfPlentyMove final : public Move {
private:
    ResourceType m_first;
    ResourceType m_second;
public:
    YearOfPlentyMove(PlayerId playerId, ResourceType first, ResourceType second)
        : Move(playerId), m_first(first), m_second(second) {}

    MoveType type() const override { return MoveType::YearOfPlenty; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
};


#endif //CATAN_YEAROFPLENTYMOVE_H