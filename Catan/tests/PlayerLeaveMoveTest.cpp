//
// Created by marko-cvijetinovic on 2/6/26.
//

#include <catch2/catch_test_macros.hpp>
#include <move/turn/PlayerLeaveMove.hpp>
#include "model/GameSession.h"

#include "TestHelper.h"

TEST_CASE("PlayerLeaveMove::isValid always true", "[PlayerLeaveMove][isValid]") {
    GameSession session = make3PGame();

    PlayerLeaveMove move0(0);
    PlayerLeaveMove move1(1);
    PlayerLeaveMove move2(2);

    REQUIRE(move0.isValid(session) == true);
    REQUIRE(move1.isValid(session) == true);
    REQUIRE(move2.isValid(session) == true);
}

TEST_CASE("PlayerLeaveMove::apply marks player as left and decreases activePlayersLeft",
          "[PlayerLeaveMove][apply][GameSession]") {
    GameSession session = make3PGame();

    const int beforeActive = session.activePlayersLeft();
    REQUIRE(beforeActive == 3);

    // sanity: player is active before leaving (assuming Player defaults to active)
    REQUIRE(session.player(1).isActive() == true);

    PlayerLeaveMove leave1(1);
    leave1.apply(session);

    REQUIRE(session.activePlayersLeft() == beforeActive - 1);
    REQUIRE(session.player(1).isActive() == false);

    // other players remain active
    REQUIRE(session.player(0).isActive() == true);
    REQUIRE(session.player(2).isActive() == true);
}

TEST_CASE("PlayerLeaveMove applied via GameSession::applyMove") {
    GameSession session = make3PGame();

#ifdef TESTING
    // Keep the session in a stable phase; not strictly required because PlayerLeaveMove::isValid ignores phase.
    forcePhase(session, TurnPhase::Main);
#endif

    const int beforeActive = session.activePlayersLeft();

    PlayerLeaveMove leave2(2);
    REQUIRE(session.applyMove(leave2) == true);

    REQUIRE(session.activePlayersLeft() == beforeActive - 1);
    REQUIRE(session.player(2).isActive() == false);
}

TEST_CASE("Multiple PlayerLeaveMove calls reduce activePlayersLeft cumulatively") {
    GameSession session = make3PGame();

    REQUIRE(session.activePlayersLeft() == 3);

    PlayerLeaveMove leave1(1);
    leave1.apply(session);
    REQUIRE(session.activePlayersLeft() == 2);
    REQUIRE(session.player(1).isActive() == false);

    PlayerLeaveMove leave0(0);
    leave0.apply(session);
    REQUIRE(session.activePlayersLeft() == 1);
    REQUIRE(session.player(0).isActive() == false);

    REQUIRE(session.player(2).isActive() == true);
}
