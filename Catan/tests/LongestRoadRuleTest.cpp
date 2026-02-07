#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "rules/LongestRoadRule.h"
#include "model/GameSession.h"

static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8},TileDef{1, 0, ResourceType::Wheat, 8},{TileDef{2, 2, ResourceType::Wheat, 8}}});
    return GameSession(names, 0, 1, std::move(board), 10, "test");
}
static void placeRoad(
    GameSession& session,PlayerId p,const EdgeCoords& c) {
    auto* edge = session.board().getEdgeAt(c);
    REQUIRE(edge != nullptr);
    session.board().placeRoad(p, edge->getEdgeId());
    session.player(p).addRoad(edge);
}
static void placeRoads(
    GameSession& session,PlayerId p,const std::vector<EdgeCoords>& coords) {
    for (const auto& c : coords)
        placeRoad(session, p, c);
}
static void placeHouse(GameSession& session,PlayerId p,const NodeCoords& c) {
    auto* node = session.board().getNodeAt(c);
    REQUIRE(node != nullptr);
    session.board().placeSettlement(p, node->getNodeId());
    session.player(p).addSettlement(node);
}

TEST_CASE("LongestRoadRule Tests", "[LongestRoad]") {
    LongestRoadRule rule;
    PlayerId p0 = 0;
    PlayerId p1 = 1;

    SECTION("No one gets the title with less than 5 roads") {
        GameSession session = makeTestSession();

        std::vector<EdgeCoords> roads = {
            {{0,0}, EdgeDirection::TopRight},
            {{0,0}, EdgeDirection::Right},
            {{0,0}, EdgeDirection::BottomRight},
            {{0,0}, EdgeDirection::BottomLeft},
        };

        placeRoads(session, p0, roads);
        //placeHouse(session, p0, {{0,0}, PointDirection::Bottom});

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 4);
        REQUIRE(session.longestRoadOwner() == types::InvalidPlayerId);
    }


    SECTION("First player to reach 5 roads gets the title") {
        GameSession session = makeTestSession();

        std::vector<EdgeCoords> roads = {
            {{0,0}, EdgeDirection::TopRight},
            {{0,0}, EdgeDirection::Right},
            {{0,0}, EdgeDirection::BottomRight},
            {{0,0}, EdgeDirection::BottomLeft},
            {{0,0}, EdgeDirection::Left},
        };

        placeRoads(session, p0, roads);
        //placeHouse(session, p0, {{0,0}, PointDirection::Bottom});

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 5);
        REQUIRE(session.longestRoadOwner() == p0);
    }


    SECTION("Must have STRICTLY more roads to take the title") {
        GameSession session = makeTestSession();

        placeRoads(session, p0, {
            {{0,0}, EdgeDirection::TopRight},
            {{0,0}, EdgeDirection::Right},
            {{0,0}, EdgeDirection::BottomRight},
            {{0,0}, EdgeDirection::BottomLeft},
            {{0,0}, EdgeDirection::Left},
        });
        //placeHouse(session, p0, {{0,0}, PointDirection::Bottom});

        rule.evaluate(session);
        REQUIRE(session.longestRoadOwner() == p0);

        placeRoads(session, p1, {
            {{2,2}, EdgeDirection::TopLeft},
            {{2,2}, EdgeDirection::Left},
            {{2,2}, EdgeDirection::BottomLeft},
            {{2,2}, EdgeDirection::BottomRight},
            {{2,2}, EdgeDirection::Right},
        });
        //placeHouse(session, p1, {{2,2}, PointDirection::Bottom});


        rule.evaluate(session);
        REQUIRE(session.longestRoadOwner() == p0);

        placeRoad(session, p1, {{2,2}, EdgeDirection::TopRight});
        rule.evaluate(session);

        REQUIRE(session.longestRoadOwner() == p1);
    }


    SECTION("Enemy settlement interrupts continuous road") {
        GameSession session = makeTestSession();

        EdgeCoords e1 = {{0,0}, EdgeDirection::TopRight};
        EdgeCoords e2 = {{0,0}, EdgeDirection::Right};
        EdgeCoords e3 = {{0,0}, EdgeDirection::BottomRight};

        placeRoad(session, p0, e1);
        placeRoad(session, p0, e2);
        placeRoad(session, p0, e3);
        //placeHouse(session, p1, {{0,0}, PointDirection::Top});

        auto* edge1 = session.board().getEdgeAt(e1);
        auto* edge2 = session.board().getEdgeAt(e2);

        NodeId blockNode =
            session.board().getNodeBetweenEdges(
                edge1->getEdgeId(),
                edge2->getEdgeId()
            )->getNodeId();

        session.board().placeSettlement(p1, blockNode);

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 2);
    }


    SECTION("Cycle (hexagon) counts as longest simple path") {
        GameSession session = makeTestSession();

        placeRoads(session, p0, {
            {{0,0}, EdgeDirection::TopRight},
            {{0,0}, EdgeDirection::Right},
            {{0,0}, EdgeDirection::BottomRight},
            {{0,0}, EdgeDirection::BottomLeft},
            {{0,0}, EdgeDirection::Left},
            {{0,0}, EdgeDirection::TopLeft},
        });
        //placeHouse(session, p0, {{0,0}, PointDirection::Bottom});

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 6);
    }

    SECTION("Cycle with one stump on the side") {
        GameSession session = makeTestSession();

        placeRoads(session, p0, {
            {{0,0}, EdgeDirection::Right},
            {{0,0}, EdgeDirection::BottomRight},
            {{0,0}, EdgeDirection::BottomLeft},
            {{0,0}, EdgeDirection::Left},
            {{0,0}, EdgeDirection::TopLeft},
        });
        placeRoad(session, p0, {{1,0}, EdgeDirection::TopLeft});
        placeRoad(session, p0, {{1,0}, EdgeDirection::BottomLeft});
        //placeHouse(session, p0, {{0,0}, PointDirection::Bottom});

        rule.evaluate(session);

        REQUIRE(session.player(p0).getRoadLength() == 7);
    }

}