//
// Created by marko-cvijetinovic on 2/6/26.
//

#include <catch2/catch_test_macros.hpp>

#include "move/turn/EndTurnMove.h"
#include "model/GameSession.h"
#include "types/TypeAliases.h"

#include "TestHelper.h"

TEST_CASE("EndTurnMove::isValid requires current player and Main phase") {
    GameSession session = make3PGame();

#ifdef TESTING
    forcePhase(session, TurnPhase::Main);
    forceCurrent(session, 0);
#endif

    SECTION("Valid when current player ends turn in Main phase") {
        EndTurnMove m(0);
        REQUIRE(m.isValid(session) == true);
    }

    SECTION("Invalid when not current player") {
        EndTurnMove m(1);
        REQUIRE(m.isValid(session) == false);
    }

    SECTION("Invalid when wrong phase") {
#ifdef TESTING
        forcePhase(session, TurnPhase::RollDice);
#endif
        EndTurnMove m(0);
        REQUIRE(m.isValid(session) == false);
    }
}

TEST_CASE("GameSession::applyMove advances player and sets phase to RollDice") {
    GameSession session = make3PGame();

#ifdef TESTING
    forcePhase(session, TurnPhase::Main);
    forceCurrent(session, 0);
#endif

    const PlayerId before = session.currentPlayer();
    REQUIRE(before == 0);

    EndTurnMove end(before);
    REQUIRE(session.applyMove(end) == true);

    // After EndTurn, GameSession::advancePhaseAfterMove() does:
    // - advancePlayer()
    // - reset dev-card flag
    // - clear trades
    // - add turn in gameData
    // - set phase RollDice
    REQUIRE(session.phase() == TurnPhase::RollDice);
    REQUIRE(session.currentPlayer() != before);
    REQUIRE(session.currentPlayer() == 1); // with 3P game starting at 0, next should be 1
}


TEST_CASE("EndTurnMove skips inactive players when advancing") {
    GameSession session = make3PGame();

#ifdef TESTING
    forcePhase(session, TurnPhase::Main);
    forceCurrent(session, 0);
#endif

    // Make player 1 leave, so next active should be player 2.
    session.leavePlayer(1);
    REQUIRE(session.player(1).isActive() == false);

    EndTurnMove end0(0);
    REQUIRE(session.applyMove(end0) == true);

    REQUIRE(session.phase() == TurnPhase::RollDice);
    REQUIRE(session.currentPlayer() == 2);
}

TEST_CASE("EndTurnMove invalid move is rejected by GameSession") {
    GameSession session = make3PGame();

#ifdef TESTING
    forcePhase(session, TurnPhase::Main);
    forceCurrent(session, 0);
#endif

    const auto phaseBefore = session.phase();
    const auto playerBefore = session.currentPlayer();

    // Wrong player attempts to end turn
    EndTurnMove bad(1);
    REQUIRE(session.applyMove(bad) == false);

    REQUIRE(session.phase() == phaseBefore);
    REQUIRE(session.currentPlayer() == playerBefore);
}
