//
// Created by marko-cvijetinovic on 2/5/26.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <board/Board.h>
#include <board/factory/RandomStandardMapCreator.hpp>

#include <player/Player.h>
#include <types/TypeAliases.h>

// deterministic board for tests
static std::unique_ptr<Board> makeStandardBoard()
{
    return (new RandomStandardMapCreator)->getBoard();
}

static NodeId pickAnyNodeId(Board const& b)
{
    auto ids = b.nodeIds();
    REQUIRE_FALSE(ids.empty());
    return ids.front();
}

static EdgeId pickAnyEdgeId(Board const& b)
{
    auto ids = b.edgeIds();
    REQUIRE_FALSE(ids.empty());
    return ids.front();
}

static Node* getKnownGenericPortNode(Board& b)
{
    // From populatePorts(): NodeCoords({0,-2}, LeftTop) + Top are generic (None)
    Node* n = b.getNodeAt(NodeCoords({0,-2}, PointDirection::LeftTop));
    REQUIRE(n != nullptr);
    return n;
}

static Node* getKnown2for1PortNode(Board& b, ResourceType r)
{
    // Choose one mapping from populatePorts().
    // Wood: NodeCoords({1,-2}, Top)
    // Brick: NodeCoords({2,0}, RightTop)
    // Wool: NodeCoords({-2,2}, Bottom)
    // Wheat: NodeCoords({-1,2}, Bottom)
    // Ore: NodeCoords({1,1}, Bottom)

    switch (r) {
        case ResourceType::Wood:  {
            auto* n = b.getNodeAt(NodeCoords({1,-2}, PointDirection::Top));
            REQUIRE(n != nullptr); return n;
        }
        case ResourceType::Brick: {
            auto* n = b.getNodeAt(NodeCoords({2,0}, PointDirection::RightTop));
            REQUIRE(n != nullptr); return n;
        }
        case ResourceType::Wool:  {
            auto* n = b.getNodeAt(NodeCoords({-2,2}, PointDirection::Bottom));
            REQUIRE(n != nullptr); return n;
        }
        case ResourceType::Wheat: {
            auto* n = b.getNodeAt(NodeCoords({-1,2}, PointDirection::Bottom));
            REQUIRE(n != nullptr); return n;
        }
        case ResourceType::Ore:   {
            auto* n = b.getNodeAt(NodeCoords({1,1}, PointDirection::Bottom));
            REQUIRE(n != nullptr); return n;
        }
        default:
            FAIL("Unsupported resource for 2:1 port in test");
            return nullptr;
    }
}

TEST_CASE("ResourceHolder add/remove resources keeps counts consistent")
{
    Player p(1, "Andjela");

    // Assuming ResourceType enum contains these:
    p.addResource(ResourceType::Wood, 3);
    p.addResource(ResourceType::Brick, 1);

    REQUIRE(p.hasResource(ResourceType::Wood, 3));
    REQUIRE_FALSE(p.hasResource(ResourceType::Wood, 4));
    REQUIRE(p.hasResource(ResourceType::Brick, 1));

    p.removeResource(ResourceType::Wood, 2);
    REQUIRE(p.hasResource(ResourceType::Wood, 1));
    REQUIRE_FALSE(p.hasResource(ResourceType::Wood, 2));
}

TEST_CASE("takeRandomResource removes exactly one resource card of an existing type")
{
    Player p(1, "Andjela");

    p.addResource(ResourceType::Wood, 2);
    p.addResource(ResourceType::Brick, 1);
    p.addResource(ResourceType::Wool, 0);

    auto before = p.getResources();

    ResourceType drawn = p.takeRandomResource();

    auto after = p.getResources();

    // Total count must drop by 1
    int sumBefore = 0, sumAfter = 0;
    for (auto const& [rt, cnt] : before) sumBefore += cnt;
    for (auto const& [rt, cnt] : after)  sumAfter  += cnt;
    REQUIRE(sumAfter == sumBefore - 1);

    // The drawn type must have decreased by 1 (and must have existed with >0 before)
    REQUIRE(before[drawn] > 0);
    REQUIRE(after[drawn] == before[drawn] - 1);
}

TEST_CASE("takeRandomResources(amount) removes exactly amount resources and returns a matching pack")
{
    Player p(1, "Andjela");
    p.addResource(ResourceType::Wood, 3);
    p.addResource(ResourceType::Brick, 2);
    p.addResource(ResourceType::Wheat, 1);

    auto before = p.getResources();

    auto pack = p.takeRandomResources(4);

    auto after = p.getResources();

    int removed = 0;
    for (auto const& [rt, cnt] : pack) removed += cnt;
    REQUIRE(removed == 4);

    // For each resource type, after = before - pack
    for (auto const& [rt, cntBefore] : before) {
        int cntPack = 0;
        auto it = pack.find(rt);
        if (it != pack.end()) cntPack = it->second;
        REQUIRE(after[rt] == cntBefore - cntPack);
    }
}

TEST_CASE("has3for1Trade on a generic port node")
{
    auto board = makeStandardBoard();

    Player p(2, "Matija");

    Node* portNode = getKnownGenericPortNode(*board);

    board->placeSettlement(1, portNode->getNodeId());
    p.addSettlement(portNode);

    REQUIRE(p.has3for1Trade());
}

TEST_CASE("hasTrade(resource) on a 2:1 port for that resource")
{
    auto board = makeStandardBoard();

    Player p(1, "Andjela");

    ResourceType R = ResourceType::Wood;
    Node* portNode = getKnown2for1PortNode(*board, R);

    board->placeSettlement(1, portNode->getNodeId());
    p.addSettlement(portNode);

    REQUIRE(p.hasTrade(R));
}

TEST_CASE("minBankTradeRatio prioritizes 2:1 over 3:1 over 4:1")
{
    auto board = makeStandardBoard();

    Player p(2, "Matija");

    // Base: no trades
    REQUIRE(p.minBankTradeRatio(ResourceType::Wood) == 4);

    // Add generic port settlement => 3:1 for everything
    {
        Node* generic = getKnownGenericPortNode(*board);
        board->placeSettlement(1, generic->getNodeId());
        p.addSettlement(generic);
        REQUIRE(p.minBankTradeRatio(ResourceType::Wood) == 3);
        REQUIRE(p.minBankTradeRatio(ResourceType::Brick) == 3);
    }

    // Add 2:1 port for Wood => 2 for Wood, still 3 for others
    {
        Node* woodPort = getKnown2for1PortNode(*board, ResourceType::Wood);
        board->placeSettlement(1, woodPort->getNodeId());
        p.addSettlement(woodPort);

        REQUIRE(p.minBankTradeRatio(ResourceType::Wood) == 2);
        REQUIRE(p.minBankTradeRatio(ResourceType::Brick) == 3);
    }
}

TEST_CASE("addRoad/addSettlement/addCity reduce remaining pieces (if getters exist)")
{
    auto board = makeStandardBoard();
    Player p(1, "Andjela");

    EdgeId e = pickAnyEdgeId(*board);
    NodeId n = pickAnyNodeId(*board);

    REQUIRE(p.getNumOfRoadsLeft() == 15);
    REQUIRE(p.getNumOfSettlementsLeft() == 5);
    REQUIRE(p.getNumOfCitiesLeft() == 4);

    p.addRoad(board->getEdgeById(e));
    p.addSettlement(board->getNodeById(n));

    REQUIRE(p.getNumOfSettlementsLeft() == 4);

    p.addCity();

    REQUIRE(p.getNumOfRoadsLeft() == 14);
    REQUIRE(p.getNumOfSettlementsLeft() == 5);
    REQUIRE(p.getNumOfCitiesLeft() == 3);
}
