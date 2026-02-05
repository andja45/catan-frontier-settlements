//
// Created by andja on 16.1.26..
//

#ifndef CATAN_TURNPHASE_H
#define CATAN_TURNPHASE_H

enum class TurnPhase {
    RollDice, // Start of turn, awaits dice roll
    Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice

    // Robber,
    RoadBuilding,
    YearOfPlenty,
    Monopoly,

    DiscardCards, // Game enters in this phase when 7 is rolled and waits for all players to send their discard cards moves
    SetRobber,
    StealCard,

    InitialPlacement, // Special phase for initial placement turns (every player chooses one road and settlement)
    GameOver
};

#endif //CATAN_TURNPHASE_H