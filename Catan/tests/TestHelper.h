//
// Created by andja on 6.2.26..
//
#include <memory>
#include <vector>
#include <string>
#include <initializer_list>

#include "model/GameSession.h"
#include "board/Board.h"
#include "types/ResourceType.h"
#include "types/TypeAliases.h"

static ResourcePack pack(std::initializer_list<std::pair<ResourceType, int>> items) {
    ResourcePack p;
    for (auto& [t, a] : items)
        p[t] += a;
    return p;
}

static void giveResources(GameSession& session,
                          PlayerId playerId,
                          const ResourcePack& p) {
    session.player(playerId).addResources(p);
}

static std::unique_ptr<Board> makeTestBoard() {
    auto b = std::make_unique<Board>();

    std::vector<TileDef> tiles;
    tiles.push_back(TileDef{1, 0, ResourceType::Wood, 6});
    tiles.push_back(TileDef{0, 0, ResourceType::Desert, 7});

    b->initializeBoard(tiles);

    return b;
}

static Node* firstNode(const GameSession& s) {

    const auto& ids = s.board().getNodes();
    REQUIRE(!ids.empty());
    return ids[0].get();
}
static Node* secondNode(const GameSession& s) {

    const auto& ids = s.board().getNodes();
    REQUIRE(!ids.empty());
    return ids[1].get();
}

static TileId firstTileId(const GameSession& s) {
    auto ids = s.board().tileIds();
    REQUIRE(ids.size() >= 1);
    return ids[0];
}

static TileId secondTileId(const GameSession& s) {
    auto ids = s.board().tileIds();
    REQUIRE(ids.size() >= 2);
    return ids[1];
}

static GameSession make2PGame() {
    std::vector<std::string> names = {"Ana", "Marko"};
    return GameSession(
        names,
        /*localPlayer*/ 0,
        /*seed*/ 1,
        makeTestBoard(),
        /*winPoints*/ 10,
        "test"
    );
}

static GameSession make3PGame() {
    std::vector<std::string> names = {"Ana", "Marko", "Jovan"};
    return GameSession(
        names,
        /*localPlayer*/ 0,
        /*seed*/ 1,
        makeTestBoard(),
        /*winPoints*/ 10,
        "test"
    );
}

#ifdef TESTING
static void forcePhase(GameSession& session, TurnPhase phase) {
    session.forcePhase(phase);
}
static void forceCurrent(GameSession& s, PlayerId p) { s.forceCurrentPlayer(p); }
#endif
