#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

#include "model/GameSession.h"
#include "board/Board.h"
#include "move/build/BuildSettlementMove.h"
#include "types/ResourceType.h"

static void setPlayerResources(GameSession& session, PlayerId p, int wood, int brick, int sheep, int wheat) {
    ResourcePack rp;
    rp[ResourceType::Wood] = wood;
    rp[ResourceType::Brick] = brick;
    rp[ResourceType::Wool] = sheep;
    rp[ResourceType::Wheat] = wheat;
    session.player(p).addResources(rp);
}

static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8}});
    return GameSession(names, 0, 1, std::move(board), 10, "test");
}

TEST_CASE("BuildSettlementMove::isValid tests", "[BuildSettlementMove][isValid]")
{
    SECTION("False if node is already occupied")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 5;

        session.enterMainPhase();
        session.board().placeSettlement(1, testNode);

        BuildSettlementMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("False if node touches another building (Distance Rule)")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        NodeId neighborNode = 1;
        session.board().placeSettlement(1, neighborNode);

        BuildSettlementMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Initial Placement Phase")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterInitialPlacementPhase();

        BuildSettlementMove move(p0, testNode);
        REQUIRE(move.isValid(session) == true);
    }

    SECTION("Main Phase - Resource and Road check")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();

        SECTION("False if no resources")
        {
            BuildSettlementMove move(p0, testNode);
            REQUIRE(move.isValid(session) == false);
        }

        SECTION("False if no connecting road")
        {
            setPlayerResources(session, p0, 1, 1, 1, 1);
            BuildSettlementMove move(p0, testNode);
            REQUIRE(move.isValid(session) == false);
        }

        SECTION("True if has resources and connecting road")
        {
            setPlayerResources(session, p0, 1, 1, 1, 1);
            EdgeId roadEdge = 0;
            session.board().placeRoad(p0, roadEdge);

            BuildSettlementMove move(p0, testNode);
            REQUIRE(move.isValid(session) == true);
        }
    }
}

TEST_CASE("BuildSettlementMove::apply effects", "[BuildSettlementMove][apply]") {
    SECTION("Applying move in Main phase consumes resources and adds points")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        setPlayerResources(session, p0, 1, 1, 1, 1);
        int initialPoints = session.player(p0).getTotalPoints();

        BuildSettlementMove move(p0, testNode);

        move.apply(session);

        REQUIRE(session.player(p0).getTotalPoints() == initialPoints + 1);
        REQUIRE(session.player(p0).getNumOfResourceCards() == 0);
        REQUIRE(session.board().getNodeOwner(testNode) == p0);
    }

    SECTION("Applying move in Initial phase does NOT consume resources")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0 ;

        session.enterInitialPlacementPhase();
        setPlayerResources(session, p0, 1, 1, 1, 1);

        BuildSettlementMove move(p0, testNode);
        session.applyMove(move);

        REQUIRE(session.player(p0).getNumOfResourceCards() == 4);
        REQUIRE(session.board().getNodeOwner(testNode) == p0);
    }
}