#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>

#include "model/GameSession.h"
#include "board/Board.h"
#include "move/robber/DiscardCardsMove.h"

#include "types/TypeAliases.h"
#include "types/ResourceType.h"

static ResourcePack pack(std::initializer_list<std::pair<ResourceType, int>> items)
{
    ResourcePack p;
    for (auto& [t, a] : items) p[t] += a;
    return p;
}

static void giveResources(GameSession& session, PlayerId playerId, const ResourcePack& p)
{
    session.player(playerId).addResources(p);
}

static std::unique_ptr<Board> makeAnyBoard()
{
    auto b = std::make_unique<Board>();

    std::vector<TileDef> tiles;
    tiles.push_back(TileDef{0, 0, ResourceType::Sea, 6}); // keep Sea if that's your enum value
    b->initializeBoard(tiles);

    return b;
}

static GameSession make2P()
{
    std::vector<std::string> names = {"Ana", "Marko"};
    return GameSession(
        names,
        /*localPlayer*/ 0,
        /*seed*/ 1,
        makeAnyBoard(),
        /*winPoints*/ 10,
        "test"
    );
}

TEST_CASE("DiscardCardsMove::isValid tests", "[DiscardCardsMove][isValid]")
{
    SECTION("Given phase is not DiscardCards, when validating, then false is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;

        giveResources(session, p, pack({
            {ResourceType::Wood, 4},
            {ResourceType::Brick, 4},
        }));
        REQUIRE(session.player(p).getNumOfResourceCards() == 8);
        REQUIRE(session.playerMustDiscard(p) == true);

        DiscardCardsMove move(p, pack({{ResourceType::Wood, 4}}));

        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given player has <= 7 cards, when validating, then false is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;

        session.enterDiscardCardsPhase();
        giveResources(session, p, pack({{ResourceType::Wood, 7}}));

        REQUIRE(session.player(p).getNumOfResourceCards() == 7);
        REQUIRE(session.playerMustDiscard(p) == false);

        DiscardCardsMove move(p, pack({{ResourceType::Wood, 3}}));

        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given DiscardCards phase and player must discard, when discarded total is not exactly half, then false is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;

        session.enterDiscardCardsPhase();

        // 8 total => must discard 4
        giveResources(session, p, pack({
            {ResourceType::Wood, 4},
            {ResourceType::Brick, 4},
        }));
        REQUIRE(session.player(p).getNumOfResourceCards() == 8);
        REQUIRE(session.playerMustDiscard(p) == true);

        DiscardCardsMove tooFew(p, pack({{ResourceType::Wood, 3}}));
        DiscardCardsMove tooMany(p, pack({{ResourceType::Wood, 5}}));

        REQUIRE(tooFew.isValid(session) == false);
        REQUIRE(tooMany.isValid(session) == false);
    }

    SECTION("Given required amount is correct but player doesn't have those resources, when validating, then false is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;

        session.enterDiscardCardsPhase();

        giveResources(session, p, pack({{ResourceType::Wood, 8}}));
        REQUIRE(session.playerMustDiscard(p) == true);

        DiscardCardsMove move(p, pack({{ResourceType::Brick, 4}}));

        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Given DiscardCards phase and correct half discard and player has resources, when validating, then true is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;

        session.enterDiscardCardsPhase();

        // 10 total => must discard 5
        giveResources(session, p, pack({
            {ResourceType::Wood, 3},
            {ResourceType::Brick, 2},
            {ResourceType::Wool, 5},
        }));
        REQUIRE(session.player(p).getNumOfResourceCards() == 10);
        REQUIRE(session.playerMustDiscard(p) == true);

        DiscardCardsMove move(p, pack({{ResourceType::Wool, 5}}));

        REQUIRE(move.isValid(session) == true);
    }

    SECTION("Given player already discarded once, when validating again, then false is returned")
    {
        GameSession session = make2P();
        const PlayerId p = 0;
        const PlayerId p2 = 1;

        session.enterDiscardCardsPhase();
        giveResources(session, p, pack({{ResourceType::Wood, 8}}));
        giveResources(session, p2, pack({{ResourceType::Wood, 8}}));

        DiscardCardsMove first(p, pack({{ResourceType::Wood, 4}}));
        REQUIRE(session.applyMove(first) == true);
        REQUIRE(session.hasPlayerDiscarded(p) == true);

        giveResources(session, p, pack({{ResourceType::Wood, 8}}));
        const int halfNow = session.player(p).getNumOfResourceCards() / 2;

        DiscardCardsMove second(p, pack({{ResourceType::Wood, halfNow}}));

        REQUIRE(second.isValid(session) == false);
    }
}

TEST_CASE("DiscardCardsMove::apply effects", "[DiscardCardsMove][apply]")
{
    SECTION("Given a valid discard move, when applied through GameSession, then resources decrease and player is marked discarded")
    {
        GameSession session = make2P();
        const PlayerId p = 0;
        const PlayerId p2 = 1;

        session.enterDiscardCardsPhase();

        // 8 total => discard 4
        giveResources(session, p, pack({
            {ResourceType::Wood, 4},
            {ResourceType::Brick, 4},
        }));
        giveResources(session, p2, pack({
            {ResourceType::Wood, 4},
            {ResourceType::Brick, 4},
        }));
        REQUIRE(session.player(p).getNumOfResourceCards() == 8);

        const ResourcePack discardPack = pack({
            {ResourceType::Wood, 2},
            {ResourceType::Brick, 2},
        });

        DiscardCardsMove move(p, discardPack);
        REQUIRE(move.isValid(session) == true);

        const bool applied = session.applyMove(move);

        REQUIRE(applied == true);
        REQUIRE(session.hasPlayerDiscarded(p) == true);
        REQUIRE(session.player(p).getNumOfResourceCards() == 4);
        REQUIRE(session.player(p).getNumOfResourceCards(ResourceType::Wood)  == 2);
        REQUIRE(session.player(p).getNumOfResourceCards(ResourceType::Brick) == 2);
    }
}
