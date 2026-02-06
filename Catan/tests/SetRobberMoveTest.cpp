//
// Created by andja on 5.2.26..
//
#include <catch2/catch_test_macros.hpp>
#include "TestHelper.h"
#include "move/robber/SetRobberMove.h"

TEST_CASE("SetRobberMove::isValid tests", "[SetRobberMove][isValid]") {

    SECTION("Given not current player, then isValid is false") {
        GameSession session = make2PGame();
        session.enterDevCardPhase(DevCardType::Knight); // phase -> SetRobber

        const TileId t0 = firstTileId(session);

        SetRobberMove move(/*playerId*/ 1, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given wrong phase, then isValid is false") {
        GameSession session = make2PGame();

        const TileId t0 = firstTileId(session);

        SetRobberMove move(/*playerId*/ 0, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given invalid tile id, then isValid is false") {
        GameSession session = make2PGame();
        session.enterDevCardPhase(DevCardType::Knight);

        SetRobberMove move(/*playerId*/ 0, types::InvalidTileId);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given robber already on that tile, then isValid is false") {
        GameSession session = make2PGame();
        session.enterDevCardPhase(DevCardType::Knight);

        Board& b = session.board();
        const TileId t0 = firstTileId(session);

        b.placeRobber(t0);

        SetRobberMove move(/*playerId*/ 0, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given SetRobber phase, current player, and different tile, then isValid is true") {
        GameSession session = make2PGame();
        session.enterDevCardPhase(DevCardType::Knight);

        Board& b = session.board();
        const TileId t0 = firstTileId(session);
        const TileId t1 = secondTileId(session);

        b.placeRobber(t0);

        SetRobberMove move(/*playerId*/ 0, t1);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("SetRobberMove::apply effects", "[SetRobberMove][apply]") {
    SECTION("When applied, robber is placed on the chosen tile") {
        GameSession session = make2PGame();
        session.enterDevCardPhase(DevCardType::Knight);

        Board& b = session.board();
        const TileId t0 = firstTileId(session);
        const TileId t1 = secondTileId(session);

        b.placeRobber(t0);
        REQUIRE(b.isRobberOnTile(t0) == true);

        SetRobberMove move(/*playerId*/ 0, t1);
        REQUIRE(move.isValid(session) == true);

        move.apply(session);

        REQUIRE(b.isRobberOnTile(t1) == true);
    }
}
