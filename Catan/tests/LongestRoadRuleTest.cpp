#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "rules/LongestRoadRule.h"
#include "model/GameSession.h"

static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8}});
    return GameSession(names, 0, 1, std::move(board), 10, "test");
}

TEST_CASE("LongestRoadRule Tests", "[LongestRoad]") {
    GameSession session = makeTestSession();
    LongestRoadRule rule;
    PlayerId p0 = 0;
    PlayerId p1 = 1;

    SECTION("No one gets the title with less than 5 roads") {
        for (int i = 0; i < 4; i++) session.board().placeRoad(p0, i);

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 4);
        REQUIRE(session.longestRoadOwner() == types::InvalidPlayerId);
    }

    SECTION("First player to reach 5 roads gets the title") {
        for (int i = 0; i < 5; i++) session.board().placeRoad(p0, i);

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 5);
        REQUIRE(session.longestRoadOwner() == p0);
    }

    SECTION("Must have STREICTLY more roads to take the title") {
        for (int i = 0; i < 5; i++) session.board().placeRoad(p0, i);
        rule.evaluate(session);

        for (int i = 10; i < 15; i++) session.board().placeRoad(p1, i);
        rule.evaluate(session);

        REQUIRE(session.longestRoadOwner() == p0);

        session.board().placeRoad(p1, 15);
        rule.evaluate(session);
        REQUIRE(session.longestRoadOwner() == p1);
    }

    SECTION("Enemy settlement interrupts the road") {
        session.board().placeRoad(p0, 0);
        session.board().placeRoad(p0, 1);
        session.board().placeRoad(p0, 2);

        NodeId middleNode = rule.getCommonNode(session, 0, 1);
        session.board().placeSettlement(p1, middleNode);

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 2);
    }

    SECTION("Cycles (Hexagon) are counted correctly") {
        for (int i = 20; i < 26; i++) session.board().placeRoad(p0, i);

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 6);
    }
}