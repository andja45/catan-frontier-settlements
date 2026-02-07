#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <vector>

#include "model/GameSession.h"
#include "board/Board.h"
#include "move/build/BuildCityMove.h"
#include "types/ResourceType.h"
#include "move/build/BuildSettlementMove.h"

static void setPlayerResources(GameSession& session, PlayerId p, int wood, int brick, int sheep, int wheat, int ore) {
    ResourcePack rp;
    rp[ResourceType::Wood] = wood;
    rp[ResourceType::Brick] = brick;
    rp[ResourceType::Wool] = sheep;
    rp[ResourceType::Wheat] = wheat;
    rp[ResourceType::Ore] = ore;
    session.player(p).addResources(rp);
}

static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8}});

    return GameSession(names, 0, 1, std::move(board), 10, "test");
}

TEST_CASE("BuildCityMove::isValid tests", "[BuildCityMove][isValid]")
{
    SECTION("False if it is not player's turn")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();

        BuildSettlementMove settlementSetup(p0, testNode);
        settlementSetup.apply(session);

        setPlayerResources(session, p0, 10, 10, 10, 10, 10);

        BuildCityMove move(0, testNode);

        REQUIRE(move.isValid(session) == (session.currentPlayer() == 0));
    }

    SECTION("False if phase is not Main")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterDiscardCardsPhase();
        BuildCityMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("False if player has no cities left")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        while(session.player(p0).hasCityLeft()) {
            session.player(p0).addCity();
        }

        BuildCityMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("False if player lacks resources")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();

        BuildCityMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("False if node is not a settlement or not owned by player")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        setPlayerResources(session, p0, 0, 0, 0, 2, 3);

        BuildCityMove move(p0, testNode);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("True if all conditions are met")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        setPlayerResources(session, p0, 0, 0, 0, 2, 3);

        session.board().placeSettlement(p0, testNode);

        BuildCityMove move(p0, testNode);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("BuildCityMove::apply effects", "[BuildCityMove][apply]") {
    SECTION("Applying move correctly updates board, resources and points")
    {
        GameSession session = makeTestSession();
        PlayerId p0 = 0;
        NodeId testNode = 0;

        session.enterMainPhase();
        setPlayerResources(session, p0, 0, 0, 0, 2, 3);
        session.board().placeSettlement(p0, testNode);

        int initialPoints = session.player(p0).getTotalPoints();

        BuildCityMove move(p0, testNode);
        session.applyMove(move);

        REQUIRE(session.player(p0).getNumOfResourceCards(ResourceType::Wheat) == 0);
        REQUIRE(session.player(p0).getNumOfResourceCards(ResourceType::Ore) == 0);

        REQUIRE(session.player(p0).getTotalPoints() == initialPoints + 1);
        REQUIRE(session.board().isNodeCity(testNode) == true);
        REQUIRE(session.board().getNodeOwner(testNode) == p0);
    }
}