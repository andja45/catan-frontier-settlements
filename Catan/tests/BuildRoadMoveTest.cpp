#include <catch2/catch_test_macros.hpp>
#include "move/build/BuildRoadMove.h"
#include "model/GameSession.h"
#include "board/Board.h"
#include "player/Player.h"
#include "move/build/BuildSettlementMove.h"


static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8}});
    return GameSession(names, 0, 1, std::move(board), 10, "test");
}

TEST_CASE("BuildRoadMove - Comprehensive Test Suite", "[move][road]") {

    SECTION("1. Initial Placement Phase Logic") {

        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        PlayerId p1 = 1;

        session.enterInitialPlacementPhase();

        NodeId nId = 0;

        REQUIRE(session.board().isNodeFree(nId));

        BuildSettlementMove settlementMove(p0, nId);
        if (settlementMove.isValid(session)) {
            session.applyMove(settlementMove);
        }


        REQUIRE(session.lastMoveType() == MoveType::BuildSettlement);

        Node* lastNode = session.board().getNodeById(nId);
        auto edges = lastNode->getIncidentEdges();
        REQUIRE(!edges.empty());
        EdgeId touchingEdge = (*edges.begin())->getEdgeId();

        BuildRoadMove roadMove(p0, touchingEdge);

        REQUIRE(roadMove.isValid(session) == true);

        BuildRoadMove roadMove2(p1, touchingEdge);

        REQUIRE(roadMove2.isValid(session) == false);
    }


    SECTION("2. Main Phase Logic (Resources & Connectivity)") {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;

        session.enterMainPhase();
        session.player(p0).addResources(Costs::Road);

        EdgeId existingRoad = 1;
        EdgeId newRoad = 2;
        session.board().placeRoad(p0, existingRoad);
        session.player(p0).addRoad(session.board().getEdgeById(existingRoad));

        BuildRoadMove move(p0, newRoad);
        if (session.board().edgeTouchesPlayersRoad(p0, newRoad)) {
            REQUIRE(move.isValid(session) == true);
        }

        session.player(p0).removeResources(Costs::Road);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("3. Road Building Phase (Dev Card)") {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;

        session.enterRoadBuildingPhase();

        REQUIRE(session.player(p0).hasResources(Costs::Road) == false);

        EdgeId existingRoad = 1;
        EdgeId newRoad = 2;
        session.board().placeRoad(p0, existingRoad);
        session.player(p0).addRoad(session.board().getEdgeById(existingRoad));

        BuildRoadMove move(p0, newRoad);
        if (session.board().edgeTouchesPlayersRoad(p0, newRoad)) {
            REQUIRE(move.isValid(session) == true);
        }
    }

    SECTION("4. Apply - State Changes") {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        EdgeId roadId = 2;

        session.enterMainPhase();
        session.player(p0).addResources(Costs::Road);
        int startRoads = session.player(p0).getNumOfRoadsLeft();

        BuildRoadMove move(p0, roadId);
        move.apply(session);

        REQUIRE(session.player(p0).hasResources(Costs::Road) == false);
        REQUIRE(session.player(p0).getNumOfRoadsLeft() == startRoads - 1);

        auto pRoads = session.player(p0).getRoads();
        bool found = false;
        for(auto r : pRoads)
            if(r->getEdgeId() == roadId)
                found = true;
        REQUIRE(found == true);
    }

    SECTION("5. allValid - Stress Test") {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;

        session.enterMainPhase();
        session.player(p0).addResources(Costs::Road);
        session.board().placeRoad(p0, 1);
        session.player(p0).addRoad(session.board().getEdgeById(1));

        BuildRoadMove move(p0, 0);
        auto validEdges = move.allValid(session);

        for (EdgeId id : validEdges) {
            BuildRoadMove check(p0, id);
            REQUIRE(check.isValid(session) == true);
        }
    }
}