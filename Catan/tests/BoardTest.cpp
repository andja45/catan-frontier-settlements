//
// Created by marko-cvijetinovic on 2/5/26.
//

#include <algorithm>
#include <catch2/catch_test_macros.hpp>

#include <board/Board.h>
#include <board/factory/RandomStandardMapCreator.hpp>

static std::vector<TileDef> makeMiniBoard()
{
    return {
        {0, 0, ResourceType::Desert, 0},
        {1, 0, ResourceType::Wood,   8},
    };
}

static std::vector<TileDef> makeRadius2Board()
{
    std::vector<TileDef> out;
    out.reserve(19);

    // axial hex of radius 2: q in [-2..2], r in max(-2, -q-2)..min(2, -q+2)
    int num = 2;
    for (int q = -2; q <= 2; ++q) {
        int rmin = std::max(-2, -q - 2);
        int rmax = std::min( 2, -q + 2);
        for (int r = rmin; r <= rmax; ++r) {
            TileDef t;
            t.q = q;
            t.r = r;

            if (q == 0 && r == 0) {
                t.res = ResourceType::Desert;
                t.number = 0;
            } else {
                t.res = ResourceType::Wood;          // keep resources simple
                t.number = num;                      // keep numbers valid
                num++;
                if (num == 7) num++;
                if (num > 12) num = 2;
            }

            out.push_back(t);
        }
    }
    return out;
}

TEST_CASE("Board initializes tiles and allows lookup by coordinate")
{
    Board b;
    auto tiles = makeRadius2Board();
    b.initializeBoard(tiles);

    for (auto const& t : tiles) {
        AxialCoords ax(t.q, t.r);
        auto* tile = b.getTileAt(ax);
        REQUIRE(tile != nullptr);
        REQUIRE(tile->getNumber() == t.number);
        REQUIRE(tile->getResourceType() == t.res);
    }

    REQUIRE_FALSE(b.tileIds().empty());
    REQUIRE_FALSE(b.nodeIds().empty());
    REQUIRE_FALSE(b.edgeIds().empty());
}

TEST_CASE("Board does not duplicate nodes/edges across adjacent tiles")
{
    Board b;
    b.initializeBoard(makeMiniBoard());

    // Two tiles would create 12 nodes and 12 edges if duplicated.
    // In reality they share some; counts should be strictly less.
    REQUIRE(b.nodeIds().size() < 12);
    REQUIRE(b.edgeIds().size() < 12);
}

TEST_CASE("Edges connect two nodes and endpoints know their edges")
{
    Board b;
    b.initializeBoard(makeRadius2Board());

    for (auto edgeId : b.edgeIds()) {
        auto* edge = b.getEdgeById(edgeId);
        REQUIRE(edge != nullptr);

        auto nodes = b.getNodesAdjacentToEdge(edgeId);
        REQUIRE(nodes.size() == 2);
        REQUIRE(nodes[0] != nullptr);
        REQUIRE(nodes[1] != nullptr);

        auto n0 = nodes[0]->getNodeId();
        auto n1 = nodes[1]->getNodeId();

        REQUIRE(b.edgeTouchesNode(n0, edgeId));
        REQUIRE(b.edgeTouchesNode(n1, edgeId));

        auto e0 = b.getEdgesAdjacentToNode(n0);
        auto e1 = b.getEdgesAdjacentToNode(n1);

        auto hasEdge = [edgeId](std::vector<Edge*> const& es) {
            return std::any_of(es.begin(), es.end(), [edgeId](Edge* e){ return e && e->getEdgeId() == edgeId; });
        };

        REQUIRE(hasEdge(e0));
        REQUIRE(hasEdge(e1));
    }
}

TEST_CASE("Placing roads/settlements updates ownership queries")
{
    Board b;
    b.initializeBoard(makeRadius2Board());

    auto someEdge = b.edgeIds().front();
    auto endpoints = b.getNodesAdjacentToEdge(someEdge);
    REQUIRE(endpoints.size() == 2);

    auto someNode = endpoints.front()->getNodeId();

    REQUIRE(b.isEdgeFree(someEdge));
    b.placeRoad(1, someEdge);
    REQUIRE_FALSE(b.isEdgeFree(someEdge));
    REQUIRE(b.getEdgeOwner(someEdge) == 1);

    REQUIRE(b.isNodeFree(someNode));
    b.placeSettlement(2, someNode);
    REQUIRE_FALSE(b.isNodeFree(someNode));
    REQUIRE(b.getNodeOwner(someNode) == 2);
    REQUIRE(b.isNodeSettlement(someNode));
}

TEST_CASE("edgeTouchesPlayersBuilding becomes true when a player owns an endpoint")
{
    Board b;
    b.initializeBoard(makeRadius2Board());

    auto e = b.edgeIds().front();
    auto endpoints = b.getNodesAdjacentToEdge(e);
    auto n = endpoints.front()->getNodeId();

    REQUIRE_FALSE(b.edgeTouchesPlayersBuilding(7, e));
    b.placeSettlement(7, n);
    REQUIRE(b.edgeTouchesPlayersBuilding(7, e));
}

TEST_CASE("nodeTouchesAnyBuilding becomes true when an adjacent node is occupied")
{
    Board b;
    b.initializeBoard(makeRadius2Board());

    // Find a node with at least one neighbor
    NodeId a = b.nodeIds().front();
    auto neigh = b.getNodesAdjacentToNode(a);
    REQUIRE_FALSE(neigh.empty());

    NodeId bNode = neigh.front()->getNodeId();

    REQUIRE_FALSE(b.nodeTouchesAnyBuilding(a));
    b.placeSettlement(1, bNode);
    REQUIRE(b.nodeTouchesAnyBuilding(a));
}

TEST_CASE("edgeTouchesPlayersRoad checks roads adjacent to the edge endpoints, not nodeId==edgeId")
{
    Board b;
    b.initializeBoard(makeRadius2Board());

    EdgeId e0 = b.edgeIds().front();

    // Find edges incident to e0
    auto incident = b.getIncidentEdges(e0);
    REQUIRE_FALSE(incident.empty());

    // Place a road on one of those incident edges for player 1
    EdgeId incidentEdgeId = incident.front()->getEdgeId();
    b.placeRoad(1, incidentEdgeId);

    // Correct behavior: e0 "touches a player's road" via shared node.
    REQUIRE(b.edgeTouchesPlayersRoad(1, e0));
}