//
// Created by andja on 10.12.25..
//

#ifndef MOVE_H
#define MOVE_H

enum class MoveType {
    RollDice,
    EndTurn,

    BuildRoad,
    BuildSettlement,
    BuildCity,

    SetRobber,
    StealCard, // in one move?
    DiscardCards, // if more than 7 cards

    BankTrade,
    PlayerTrade,

    BuyDevCard,
    PlayDevCard,

    InvalidMoveType
};

#include "../types/TypeAliases.h"
#include "../types/Costs.h"
#include "../types/ResourceType.h"

class GameSession;

// move is our basic unit for both game logic execution and player communication, containing action that player wants to do, it can be validated and applied to game session
// its used both for storing data about player intentions, incrementally updating game model based on players actions, and validating is that move possible which we also use two enable or disable tools in user interface
// this enables us to send as little information on network as possible and keep everybody's game in sync
class Move {
public:
    virtual ~Move() = default;

    virtual MoveType type() const = 0;
    virtual bool isValid(const GameSession&) const = 0;
    virtual void apply(GameSession&) const = 0;

    virtual bool providesAllValid() const { return false; }
};


#endif //MOVE_H