#define TESTING
#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include "move/trade/BankTradeMove.h"
#include "model/GameSession.h"
#include "board/Node.h"

#include "model/GameSession.h"
#include "board/Board.h"

#include "types/TypeAliases.h"
#include "types/ResourceType.h"

#include "TestHelper.h"



TEST_CASE("BankTradeMove has correct type") {
    BankTradeMove move(0, ResourceType::Wood, ResourceType::Brick);

    REQUIRE(move.type() == MoveType::BankTrade);
}
TEST_CASE("Move::isValid tests", "[DiscardCardsMove][isValid]")
{
    SECTION("BankTradeMove is valid when player has enough resources") {
        auto session = make2PGame();
        auto& player = session.player(0);

        player.addResource(ResourceType::Wood, 4);

        forceCurrent(session, player.getPlayerId());

        forcePhase(session, TurnPhase::Main);

        BankTradeMove move(0, ResourceType::Wood, ResourceType::Brick);

        REQUIRE(move.isValid(session));
    }
    SECTION("BankTradeMove is invalid when player lacks resources") {
        auto session = make2PGame();
        auto& player = session.player(0);

        player.addResource(ResourceType::Wood, 3);

        forceCurrent(session, player.getPlayerId());

        forcePhase(session, TurnPhase::Main);

        BankTradeMove move(0, ResourceType::Wood, ResourceType::Brick);

        REQUIRE_FALSE(move.isValid(session));
    }
    SECTION("BankTradeMove is invalid when give and receive are the same") {
        auto session = make2PGame();
        auto& player = session.player(0);

        player.addResource(ResourceType::Ore, 4);

        BankTradeMove move(0, ResourceType::Ore, ResourceType::Ore);

        REQUIRE_FALSE(move.isValid(session));
    }
}

TEST_CASE("Applying BankTradeMove exchanges resources correctly") {
    auto session = make2PGame();
    auto& player = session.player(0);

    player.addResource(ResourceType::Wheat, 4);
    forceCurrent(session, player.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    BankTradeMove move(0, ResourceType::Wheat, ResourceType::Wool);

    REQUIRE(move.isValid(session));

    move.apply(session);

    auto resources = player.getResources();
    REQUIRE(resources[ResourceType::Wheat]==0);
    REQUIRE(resources[ResourceType::Wool]==1);
}


TEST_CASE("Port ratios working as intended") {
    SECTION("Two for 1 ports")
    {
        auto session = make2PGame();
        auto& player = session.player(0);

        auto portNode = firstNode(session);
        portNode->setTrade(ResourceType::Brick);

        player.addSettlement(portNode);
        REQUIRE(player.has2for1Trade(ResourceType::Brick));
        player.addResource(ResourceType::Brick, 4);


        forceCurrent(session, player.getPlayerId());

        forcePhase(session, TurnPhase::Main);

        BankTradeMove move(0, ResourceType::Brick, ResourceType::Ore);

        REQUIRE(move.isValid(session));

        move.apply(session);
        auto resources = player.getResources();
        REQUIRE(resources[ResourceType::Brick] == 2);
        REQUIRE(resources[ResourceType::Ore] == 1);
    }
    SECTION("3:1 ports")
    {
        auto session = make2PGame();
        auto& player = session.player(0);

        auto portNode = firstNode(session);
        portNode->setTrade(ResourceType::None);

        player.addSettlement(portNode);
        REQUIRE(player.has3for1Trade());
        player.addResource(ResourceType::Brick, 4);

        forceCurrent(session, player.getPlayerId());

        forcePhase(session, TurnPhase::Main);

        BankTradeMove move(0, ResourceType::Brick, ResourceType::Ore);

        REQUIRE(move.isValid(session));

        move.apply(session);
        auto resources = player.getResources();
        REQUIRE(resources[ResourceType::Brick] == 1);
        REQUIRE(resources[ResourceType::Ore] == 1);
    }
    SECTION("Both ports")
    {
        auto session = make2PGame();
        auto& player = session.player(0);

        forceCurrent(session, player.getPlayerId());

        forcePhase(session, TurnPhase::Main);


        auto portNode = firstNode(session);
        portNode->setTrade(ResourceType::Brick);

        player.addSettlement(portNode);

        auto portNode2 = secondNode(session);
        portNode2->setTrade(ResourceType::None);

        player.addSettlement(portNode2);

        REQUIRE(player.has2for1Trade(ResourceType::Brick));
        REQUIRE(player.has3for1Trade());
        player.addResource(ResourceType::Brick, 4);
        player.addResource(ResourceType::Wool, 4);

        BankTradeMove move(0, ResourceType::Brick, ResourceType::Ore);
        REQUIRE(move.isValid(session));
        move.apply(session);
        BankTradeMove move2(0, ResourceType::Wool, ResourceType::Wheat);
        REQUIRE(move2.isValid(session));
        move2.apply(session);

        auto resources = player.getResources();
        REQUIRE(resources[ResourceType::Brick] == 2);
        REQUIRE(resources[ResourceType::Wool] == 1);
        REQUIRE(resources[ResourceType::Wheat] == 1);
        REQUIRE(resources[ResourceType::Ore] == 1);
    }
}


