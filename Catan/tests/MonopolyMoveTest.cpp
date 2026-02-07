//
// Created by andja on 6.2.26..
//
#include <catch2/catch_test_macros.hpp>
#include "TestHelper.h"
#include "move/devcard/MonopolyMove.h"

TEST_CASE("MonopolyMove::isValid", "[MonopolyMove][isValid]") {
    GameSession session = make3PGame();
    const PlayerId p0 = session.currentPlayer();

    SECTION("Not current player -> false") {
        session.enterDevCardPhase(DevCardType::Monopoly);
        MonopolyMove move(/*playerId*/ 1, ResourceType::Wood);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Wrong phase -> false") {
        MonopolyMove move(p0, ResourceType::Wood);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("ResourceType::None -> false") {
        session.enterDevCardPhase(DevCardType::Monopoly);
        MonopolyMove move(p0, ResourceType::None);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("All good -> true") {
        session.enterDevCardPhase(DevCardType::Monopoly);
        MonopolyMove move(p0, ResourceType::Wood);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("MonopolyMove::apply takes all chosen resource from others", "[MonopolyMove][apply]") {
    GameSession session = make3PGame();
    const PlayerId p0 = session.currentPlayer();
    const PlayerId p1 = 1;
    const PlayerId p2 = 2;

    session.enterDevCardPhase(DevCardType::Monopoly);

    giveResources(session, p1, pack({{ResourceType::Wood, 3}, {ResourceType::Brick, 5}}));
    giveResources(session, p2, pack({{ResourceType::Wood, 2}}));

    const int p0_before = session.player(p0).getNumOfResourceCards(ResourceType::Wood);
    const int p1_before = session.player(p1).getNumOfResourceCards(ResourceType::Wood);
    const int p2_before = session.player(p2).getNumOfResourceCards(ResourceType::Wood);

    MonopolyMove move(p0, ResourceType::Wood);
    REQUIRE(move.isValid(session) == true);

    move.apply(session);

    REQUIRE(session.player(p1).getNumOfResourceCards(ResourceType::Wood) == 0);
    REQUIRE(session.player(p2).getNumOfResourceCards(ResourceType::Wood) == 0);
    REQUIRE(session.player(p0).getNumOfResourceCards(ResourceType::Wood) ==
            p0_before + p1_before + p2_before);

    // other resources are unchanged
    REQUIRE(session.player(p1).getNumOfResourceCards(ResourceType::Brick) == 5);
}

TEST_CASE("MonopolyMove::apply does nothing if nobody has that resource", "[MonopolyMove][apply]") {
    GameSession session = make3PGame();
    const PlayerId p0 = session.currentPlayer();
    const PlayerId p1 = 1;
    const PlayerId p2 = 2;

    session.enterDevCardPhase(DevCardType::Monopoly);

    giveResources(session, p1, pack({{ResourceType::Wood, 2}}));
    giveResources(session, p2, pack({{ResourceType::Brick, 4}}));

    const int ore_before = session.player(p0).getNumOfResourceCards(ResourceType::Ore);

    MonopolyMove move(p0, ResourceType::Ore);
    REQUIRE(move.isValid(session) == true);

    move.apply(session);

    REQUIRE(session.player(p0).getNumOfResourceCards(ResourceType::Ore) == ore_before);
    REQUIRE(session.player(p1).getNumOfResourceCards(ResourceType::Wood) == 2);
    REQUIRE(session.player(p2).getNumOfResourceCards(ResourceType::Brick) == 4);
}