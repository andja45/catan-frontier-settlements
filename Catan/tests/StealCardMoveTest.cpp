//
// Created by andja on 5.2.26..
//

#include <catch2/catch_test_macros.hpp>
#include "TestHelper.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"

static bool placeVictimBuildingTouchingTile(Board& board, PlayerId victim, TileId tileId) {
    Tile* target = board.getTileById(tileId);
    if (!target) return false;

    for (Node* node : target->getAdjacentNodes()) {
        if (node->isEmpty()) {
            board.placeSettlement(victim, node->getNodeId());
            return true;
        }
    }
    return false;
}

TEST_CASE("StealCardMove flow after SetRobberMove (real GameSession transitions)",
          "[StealCardMove][phase][GameSession]") {
    GameSession session = make2PGame();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(session);

    session.enterDevCardPhase(DevCardType::Knight);
    REQUIRE(session.phase() == TurnPhase::SetRobber);

    giveResources(session, victim, pack({{ResourceType::Wood, 2}}));
    REQUIRE(placeVictimBuildingTouchingTile(board, victim, t0) == true);

    SetRobberMove setRobber(thief, t0);
    REQUIRE(session.applyMove(setRobber) == true);

    REQUIRE(session.phase() == TurnPhase::StealCard);

    StealCardMove steal(thief, victim);
    REQUIRE(steal.isValid(session) == true);
}

TEST_CASE("StealCardMove::isValid conditions", "[StealCardMove][isValid]") {
    GameSession session = make2PGame();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(session);

    session.enterDevCardPhase(DevCardType::Knight);
    REQUIRE(session.phase() == TurnPhase::SetRobber);

    giveResources(session, victim, pack({{ResourceType::Brick, 1}}));
    REQUIRE(placeVictimBuildingTouchingTile(board, victim, t0) == true);

    REQUIRE(session.applyMove(SetRobberMove(thief, t0)) == true);
    REQUIRE(session.phase() == TurnPhase::StealCard);

    SECTION("Cannot steal from self") {
        StealCardMove move(thief, thief);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Invalid victim id") {
        StealCardMove move(thief, types::InvalidPlayerId);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Victim has 0 cards -> false") {
        Player& v = session.player(victim);
        v.removeResources(pack({{ResourceType::Brick, v.getNumOfResourceCards()}})); // if you have such usage
        StealCardMove move(thief, victim);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Victim must touch robber tile") {
        REQUIRE(board.tileTouchesPlayerBuilding(victim, board.robberTileId()) == true);
        StealCardMove move(thief, victim);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("StealCardMove::apply changes totals by 1", "[StealCardMove][apply]") {
    GameSession session = make2PGame();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(session);

    session.enterDevCardPhase(DevCardType::Knight);
    giveResources(session, victim, pack({{ResourceType::Wood, 1}, {ResourceType::Brick, 1}}));
    REQUIRE(placeVictimBuildingTouchingTile(board, victim, t0) == true);
    REQUIRE(session.applyMove(SetRobberMove(thief, t0)) == true);
    REQUIRE(session.phase() == TurnPhase::StealCard);

    StealCardMove move(thief, victim);
    REQUIRE(move.isValid(session) == true);

    const int thiefBefore  = session.player(thief).getNumOfResourceCards();
    const int victimBefore = session.player(victim).getNumOfResourceCards();

    move.apply(session);

    REQUIRE(session.player(thief).getNumOfResourceCards() == thiefBefore + 1);
    REQUIRE(session.player(victim).getNumOfResourceCards() == victimBefore - 1);
}
