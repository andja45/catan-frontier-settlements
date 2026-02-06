//
// Created by marko-cvijetinovic on 2/6/26.
//

#define TESTING
#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include <move/turn/RollDiceMove.h>

#include "model/GameSession.h"
#include "board/Board.h"
#include "types/TypeAliases.h"
#include "types/ResourceType.h"

// ---------------- Helpers ----------------

static ResourcePack pack(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack p;
    for (auto& [t, a] : items) p[t] += a;
    return p;
}

// A small board crafted so we can control dice outcomes.
// - one tile with number 8 (so 4+4 works)
// - one tile with number 7 (so 3+4 triggers discard)
static std::unique_ptr<Board> makeDiceBoard() {
    auto b = std::make_unique<Board>();
    std::vector<TileDef> tiles;
    tiles.push_back(TileDef{0, 0, ResourceType::Wood,   8}); // will produce resource when 8 rolled
    tiles.push_back(TileDef{1, 0, ResourceType::Desert, 7}); // 7 is special-case discard
    b->initializeBoard(tiles);
    return b;
}

static GameSession make2P_diceBoard(PlayerId localPlayer, uint32_t seed) {
    std::vector<std::string> names = {"Ana", "Marko"};
    return GameSession(names, localPlayer, seed, makeDiceBoard(), /*winPoints*/ 10, "test");
}

static bool placePlayerBuildingTouchingTile(Board& board, PlayerId owner, TileId tileId, bool city) {
    Tile* tile = board.getTileById(tileId);
    if (!tile) return false;

    for (Node* node : tile->getAdjacentNodes()) {
        if (!node) continue;
        if (node->isEmpty()) {
            board.placeSettlement(owner, node->getNodeId());
            if (city) board.placeCity(owner, node->getNodeId());
            return true;
        }
    }
    return false;
}

static TileId findTileIdWithNumber(Board& board, int number) {
    for (auto tid : board.tileIds()) {
        Tile* t = board.getTileById(tid);
        if (t && t->getNumber() == number) return tid;
    }
    FAIL("No tile with requested number on test board");
    return types::InvalidTileId;
}

// Computes the next dice SUM that a "remote verifier" expects, without advancing session RNG.
static int expectedRemoteDice(const GameSession& session) {
    auto rng = session.copyRng();
    std::uniform_int_distribution<int> d6{1, 6};
    int roll = d6(rng);
    return roll;
}

// ---------------- Tests ----------------

TEST_CASE("RollDiceMove::isValid basic gate checks", "[RollDiceMove][isValid]") {
    GameSession session = make2P_diceBoard(/*localPlayer*/0, /*seed*/1);

    REQUIRE(session.phase() == TurnPhase::InitialPlacement); // document current state

    // We can't RollDiceMove in InitialPlacement
    RollDiceMove m0(/*player*/0, /*d1*/3, /*d2*/4);
    REQUIRE_FALSE(m0.isValid(session));

    session.forcePhase(TurnPhase::RollDice);
    REQUIRE(session.currentPlayer() == 0);

    SECTION("invalid if not current player") {
        RollDiceMove mWrong(1, 3, 4);
        REQUIRE_FALSE(mWrong.isValid(session));
    }

    SECTION("invalid if dice out of range") {
        REQUIRE_FALSE(RollDiceMove(0, 0, 4).isValid(session));
        REQUIRE_FALSE(RollDiceMove(0, 7, 4).isValid(session));
        REQUIRE_FALSE(RollDiceMove(0, 3, 9).isValid(session));
    }

    SECTION("valid when current player, phase RollDice, dice in range and local") {
        REQUIRE(RollDiceMove(0, 3, 4).isValid(session));
    }
}

TEST_CASE("RollDiceMove::isValid remote verification uses copyRng sum", "[RollDiceMove][isValid][remote]") {
    // local player is 0, but we validate move for player 1 => remote verification triggers
    GameSession session = make2P_diceBoard(/*localPlayer*/0, /*seed*/123);

    // Need RollDice phase and current player = 1 for remote check
    session.forcePhase(TurnPhase::RollDice);
    session.forceCurrentPlayer(1);

    int d1 = expectedRemoteDice(session);
    int d2 = expectedRemoteDice(session);

    REQUIRE(d2 >= 1);
    REQUIRE(d2 <= 6);

    RollDiceMove ok(1, d1, d2);
    REQUIRE(ok.isValid(session));

    // Wrong sum should fail
    int bad2 = d2 == 6 ? 5 : 6;
    int bad1 = d1; // keep in range
    if (bad1 + bad2 == d1 + d2) bad2 = 4; // ensure mismatch
    RollDiceMove bad(1, bad1, bad2);
    REQUIRE_FALSE(bad.isValid(session));
    SUCCEED("Enable this test once you expose a test helper to set phase=current player to RollDice/1.");
}

TEST_CASE("RollDiceMove::apply distributes resources for matched tiles") {
    GameSession session = make2P_diceBoard(/*localPlayer*/0, /*seed*/1);
    Board& board = session.board();

    session.forcePhase(TurnPhase::RollDice);

    const PlayerId owner = 0;
    const int diceSum = 8;

    TileId t8 = findTileIdWithNumber(board, diceSum);
    REQUIRE(t8 != types::InvalidTileId);

    // Place a settlement touching that tile
    REQUIRE(placePlayerBuildingTouchingTile(board, owner, t8, /*city*/false) == true);

    Tile* tile = board.getTileById(t8);
    REQUIRE(tile != nullptr);

    ResourceType produced = tile->getResourceType();

    // ---- Roll 8 with NO robber on that tile: should produce ----
    int before = session.player(owner).getNumOfResourceCards(produced);
    int rolls8Before = session.gameData().getDiceRolls()[8];

    RollDiceMove m(owner, 4, 4); // 8
    m.apply(session);

    int after = session.player(owner).getNumOfResourceCards(produced);
    int rolls8After = session.gameData().getDiceRolls()[8];

    REQUIRE(after >= before + 1);
    REQUIRE(rolls8After - rolls8Before == 1);

    // ---- Put robber on the number-8 tile ----
    board.placeRobber(t8);

    // ---- Roll 8 again: should NOT produce (robber blocks), but roll still gets logged ----
    int beforeBlocked = session.player(owner).getNumOfResourceCards(produced);
    int rolls8BeforeBlocked = session.gameData().getDiceRolls()[8];

    m.apply(session);

    int afterBlocked = session.player(owner).getNumOfResourceCards(produced);
    int rolls8AfterBlocked = session.gameData().getDiceRolls()[8];

    REQUIRE(afterBlocked == beforeBlocked);                 // no resources gained
    REQUIRE(rolls8AfterBlocked - rolls8BeforeBlocked == 1); // still logged
}

TEST_CASE("RollDiceMove::apply gives 2 resources for city", "[RollDiceMove][apply][city]") {
    GameSession session = make2P_diceBoard(/*localPlayer*/0, /*seed*/1);
    Board& board = session.board();

    session.forcePhase(TurnPhase::RollDice);

    const PlayerId owner = 0;
    const int diceSum = 8;

    TileId t8 = findTileIdWithNumber(board, diceSum);
    REQUIRE(placePlayerBuildingTouchingTile(board, owner, t8, /*city*/true) == true);

    Tile* tile = board.getTileById(t8);
    ResourceType produced = tile->getResourceType();

    int before = session.player(owner).getNumOfResourceCards(produced);

    RollDiceMove m(owner, 4, 4);
    m.apply(session);

    int after = session.player(owner).getNumOfResourceCards(produced);
    REQUIRE(after >= before + 2);
}
