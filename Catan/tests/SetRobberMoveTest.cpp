//
// Created by andja on 5.2.26..
//

// SetRobberMoveTest.cpp

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>

#include "model/GameSession.h"
#include "board/Board.h"
#include "move/robber/SetRobberMove.h"
#include "types/TypeAliases.h"
#include "types/ResourceType.h"

static std::unique_ptr<Board> makeBoardWith2Tiles() {
    auto b = std::make_unique<Board>();

    std::vector<TileDef> tiles;
    tiles.push_back(TileDef{0, 0, ResourceType::Sea, 6});
    tiles.push_back(TileDef{1, 0, ResourceType::Desert, 8});

    b->initializeBoard(tiles);
    return b;
}

static GameSession make2P() {
    std::vector<std::string> names = {"Ana", "Marko"};
    return GameSession(
        names,
        /*localPlayer*/ 0,
        /*seed*/ 1,
        makeBoardWith2Tiles(),
        /*winPoints*/ 10,
        "test"
    );
}

static TileId firstTileId(const GameSession& s) {
    auto ids = s.board().tileIds();
    REQUIRE(ids.size() >= 1);
    return ids[0];
}

static TileId secondTileId(const GameSession& s) {
    auto ids = s.board().tileIds();
    REQUIRE(ids.size() >= 2);
    return ids[1];
}

TEST_CASE("SetRobberMove::isValid tests", "[SetRobberMove][isValid]") {

    SECTION("Given not current player, then isValid is false") {
        GameSession session = make2P();
        session.enterDevCardPhase(DevCardType::Knight); // phase -> SetRobber

        const TileId t0 = firstTileId(session);

        SetRobberMove move(/*playerId*/ 1, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given wrong phase, then isValid is false") {
        GameSession session = make2P();

        const TileId t0 = firstTileId(session);

        SetRobberMove move(/*playerId*/ 0, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given invalid tile id, then isValid is false") {
        GameSession session = make2P();
        session.enterDevCardPhase(DevCardType::Knight);

        SetRobberMove move(/*playerId*/ 0, types::InvalidTileId);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given robber already on that tile, then isValid is false") {
        GameSession session = make2P();
        session.enterDevCardPhase(DevCardType::Knight);

        Board& b = session.board();
        const TileId t0 = firstTileId(session);

        b.placeRobber(t0);

        SetRobberMove move(/*playerId*/ 0, t0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given SetRobber phase, current player, and different tile, then isValid is true") {
        GameSession session = make2P();
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
        GameSession session = make2P();
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
