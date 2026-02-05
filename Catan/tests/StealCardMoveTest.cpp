//
// Created by andja on 5.2.26..
//

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>

#include "model/GameSession.h"
#include "board/Board.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"
#include "types/TypeAliases.h"
#include "types/ResourceType.h"

static ResourcePack pack(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack p;
    for (auto& [t, a] : items) p[t] += a;
    return p;
}

static void giveResources(GameSession& session, PlayerId playerId, const ResourcePack& p) {
    session.player(playerId).addResources(p);
}

static std::unique_ptr<Board> makeSmallBoard() {
    auto b = std::make_unique<Board>();
    std::vector<TileDef> tiles;
    tiles.push_back(TileDef{0, 0, ResourceType::Sea, 6});
    tiles.push_back(TileDef{1, 0, ResourceType::Sea, 8});
    b->initializeBoard(tiles);
    return b;
}

static GameSession make2P() {
    std::vector<std::string> names = {"Ana", "Marko"};
    return GameSession(names, /*localPlayer*/ 0, /*seed*/ 1, makeSmallBoard(), /*winPoints*/ 10, "test");
}

static TileId firstTileId(const Board& b) {
    auto ids = b.tileIds();
    REQUIRE(!ids.empty());
    return ids[0];
}

static bool placeVictimBuildingTouchingTile(Board& board, PlayerId victim, TileId tileId) {
    Tile* target = board.getTileById(tileId);
    if (!target) return false;

    for (NodeId nid : board.nodeIds()) {
        if (!board.isNodeFree(nid)) continue;

        for (Tile* t : board.getTilesAdjacentToNode(nid)) {
            if (t == target) {
                board.placeSettlement(victim, nid);
                return true;
            }
        }
    }
    return false;
}

TEST_CASE("StealCardMove flow after SetRobberMove (real GameSession transitions)",
          "[StealCardMove][phase][GameSession]") {
    GameSession session = make2P();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(board);

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
    GameSession session = make2P();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(board);

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
        REQUIRE(board.tileTouchesPlayerBuilding(victim, board.robberTile()) == true);
        StealCardMove move(thief, victim);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("StealCardMove::apply changes totals by 1", "[StealCardMove][apply]") {
    GameSession session = make2P();
    Board& board = session.board();

    const PlayerId thief  = 0;
    const PlayerId victim = 1;

    const TileId t0 = firstTileId(board);

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
