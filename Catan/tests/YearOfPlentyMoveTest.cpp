//
// Created by andja on 6.2.26..
//
#include <catch2/catch_test_macros.hpp>

#include "TestHelper.h"
#include "move/devcard/YearOfPlentyMove.h"

static void drainBankToExact(Bank& bank, ResourceType t, int keep) {
    while (bank.hasResource(t, keep + 1)) { // removes one by one untill bank has exactly keep num of rss
        ResourcePack one;
        one[t] = 1;
        bank.removeResources(one);
    }
}

TEST_CASE("YearOfPlentyMove::isValid", "[YearOfPlentyMove][isValid]") {
    GameSession session = make2PGame();
    const PlayerId p0 = session.currentPlayer();

    SECTION("Not current player -> false") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        YearOfPlentyMove move(/*playerId*/ 1, ResourceType::Wood, ResourceType::Brick);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Wrong phase -> false") {
        YearOfPlentyMove move(p0, ResourceType::Wood, ResourceType::Brick);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("ResourceType::None -> false") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        YearOfPlentyMove move1(p0, ResourceType::None, ResourceType::Wood);
        YearOfPlentyMove move2(p0, ResourceType::Wood, ResourceType::None);
        REQUIRE(move1.isValid(session) == false);
        REQUIRE(move2.isValid(session) == false);
    }

    SECTION("Bank lacks first resource -> false") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        Bank& bank = session.bank();

        drainBankToExact(bank, ResourceType::Ore, 0);
        REQUIRE(bank.hasResource(ResourceType::Ore, 1) == false);

        YearOfPlentyMove move(p0, ResourceType::Ore, ResourceType::Wood);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Bank lacks second resource (different resources) -> false") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        Bank& bank = session.bank();

        drainBankToExact(bank, ResourceType::Brick, 0);
        REQUIRE(bank.hasResource(ResourceType::Brick, 1) == false);

        YearOfPlentyMove move(p0, ResourceType::Wood, ResourceType::Brick);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Same resource chosen but bank has only 1 -> false") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        Bank& bank = session.bank();

        drainBankToExact(bank, ResourceType::Wool, 1);
        REQUIRE(bank.hasResource(ResourceType::Wool, 2) == false);
        REQUIRE(bank.hasResource(ResourceType::Wool, 1) == true);

        YearOfPlentyMove move(p0, ResourceType::Wool, ResourceType::Wool);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("All requirements satisfied (two different resources) -> true") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        YearOfPlentyMove move(p0, ResourceType::Wood, ResourceType::Brick);
        REQUIRE(move.isValid(session) == true);
    }

    SECTION("All requirements satisfied (same resource, bank has >=2) -> true") {
        session.enterDevCardPhase(DevCardType::YearOfPlenty);
        YearOfPlentyMove move(p0, ResourceType::Ore, ResourceType::Ore);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("YearOfPlentyMove::apply effects", "[YearOfPlentyMove][apply]") {
    GameSession session = make2PGame();
    const PlayerId p0 = session.currentPlayer();

    session.enterDevCardPhase(DevCardType::YearOfPlenty);

    Player& player = session.player(p0);
    Bank& bank = session.bank();

    SECTION("Two different resources: player +1/+1, bank -1/-1") {
        // make bank have exactly 1 wood and 1 brick so after apply it has 0 of each
        drainBankToExact(bank, ResourceType::Wood, 1);
        drainBankToExact(bank, ResourceType::Brick, 1);
        REQUIRE(bank.hasResource(ResourceType::Wood, 1) == true);
        REQUIRE(bank.hasResource(ResourceType::Brick, 1) == true);

        const int wBefore = player.getNumOfResourceCards(ResourceType::Wood);
        const int bBefore = player.getNumOfResourceCards(ResourceType::Brick);

        YearOfPlentyMove move(p0, ResourceType::Wood, ResourceType::Brick);
        REQUIRE(move.isValid(session) == true);

        move.apply(session);

        REQUIRE(player.getNumOfResourceCards(ResourceType::Wood) == wBefore + 1);
        REQUIRE(player.getNumOfResourceCards(ResourceType::Brick) == bBefore + 1);
        REQUIRE(bank.hasResource(ResourceType::Wood, 1) == false);
        REQUIRE(bank.hasResource(ResourceType::Brick, 1) == false);
    }

    SECTION("Same resource twice: player +2, bank -2") {
        drainBankToExact(bank, ResourceType::Ore, 2);
        REQUIRE(bank.hasResource(ResourceType::Ore, 2) == true);

        const int oBefore = player.getNumOfResourceCards(ResourceType::Ore);

        YearOfPlentyMove move(p0, ResourceType::Ore, ResourceType::Ore);
        REQUIRE(move.isValid(session) == true);

        move.apply(session);

        REQUIRE(player.getNumOfResourceCards(ResourceType::Ore) == oBefore + 2);
        REQUIRE(bank.hasResource(ResourceType::Ore, 1) == false);
    }
}
