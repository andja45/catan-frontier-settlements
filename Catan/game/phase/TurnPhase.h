//
// Created by andja on 16.1.26..
//

#ifndef CATAN_TURNPHASE_H
#define CATAN_TURNPHASE_H

enum class TurnPhase {
    RollDice, // Start of turn, awaits dice roll
    Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice

    // Robber, // setrobber + stealcard TODO do they need to be two phases or in robber?
    RoadBuilding,
    YearOfPlenty,
    Monopoly,

    DiscardCards, // Game enters in this phase when 7 is rolled and waits for all players to send their discard cards moves
    SetRobber,
    StealCard,

    InitialPlacement, // Special phase for initial placement turns (every player chooses one road and settlement) //TODO consider breaking into two phases and ditch enum
   }; // drzi to stanje sve dok ne dobije signal da je uradjeno nesto sto ga menja
// tj to je stanje koje ceka, ne ono koje sledi

#endif //CATAN_TURNPHASE_H