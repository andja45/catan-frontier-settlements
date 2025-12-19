//
// Created by andja on 10.12.25..
//

#ifndef MOVE_H
#define MOVE_H

/*enum class MoveType {
    BuildRoad,
    BuildSettlement,
    BuildCity,
    Trade,
    RollDice,
    EndTurn
};*/

#include "../Types/TypeAliases.h"
#include "../Types/Costs.h"
#include "../Types/ResourceType.h"
#include "../Game/GameModel.h"
#include "../Game/GameSession.h"

class Move {
public:
    virtual ~Move() = default;

    virtual bool isValid(const GameSession&) const = 0;
    virtual void apply(GameSession&) const = 0;
};


#endif //MOVE_H