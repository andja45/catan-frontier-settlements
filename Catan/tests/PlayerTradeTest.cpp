#define TESTING
#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include "move/trade/PlayerTradeAcceptMove.h"
#include "move/trade/PlayerTradeRequestMove.h"
#include "move/trade/PlayerTradeResponseMove.h"
#include "model/GameSession.h"
#include "board/Node.h"
#include "TestHelper.h"


#include "model/GameSession.h"
#include "board/Board.h"

#include "types/TypeAliases.h"
#include "types/ResourceType.h"


TEST_CASE("PlayerTradeRequestMove is valid when requester has resources") {
    auto session = make2PGame();
    auto& player1 = session.player(0);

    ResourcePack give{{ResourceType::Wood, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};

    player1.addResource(ResourceType::Wood,1);
    //player1.addResources(give);

    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);


    PlayerTradeRequestMove move(player1.getPlayerId(), give, receive);
    move.setTradeId(1);
    REQUIRE(move.isValid(session));
}TEST_CASE("PlayerTradeRequestMove is invalid when requester lacks resources") {
    auto session = make2PGame();

    ResourcePack give{{ResourceType::Ore, 2}};
    ResourcePack receive{{ResourceType::Wheat, 1}};
    forceCurrent(session, 0);

    forcePhase(session, TurnPhase::Main);
    PlayerTradeRequestMove move(0, give, receive);

    REQUIRE_FALSE(move.isValid(session));
}
TEST_CASE("PlayerTradeRequestMove apply creates an active trade") {
    auto session = make2PGame();
    auto& player1 = session.player(0);


    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    ResourcePack give{{ResourceType::Wool, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};
    player1.addResources(give);

    PlayerTradeRequestMove move(0, give, receive);
    move.setTradeId(1);
    REQUIRE(move.isValid(session));

    move.apply(session);

    REQUIRE(session.activeTrades().size() == 1);
}

TEST_CASE("PlayerTradeResponseMove valid for non-requester") {
    auto session = make2PGame();
    auto& player1 = session.player(0);
    auto& player2   = session.player(1);

    ResourcePack give{{ResourceType::Wood, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};

    player1.addResources(give);
    player2.addResources(receive);

    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    PlayerTradeRequestMove request(player1.getPlayerId(), give, receive);
    request.apply(session);

    TradeId tradeId = session.activeTrades().front()->id();

    PlayerTradeResponseMove response(player2.getPlayerId(), tradeId);

    REQUIRE(response.isValid(session));
}

TEST_CASE("PlayerTradeAcceptMove is valid when responder can fulfill trade") {
    auto session = make2PGame();
    auto& player1 = session.player(0);
    auto& player2   = session.player(1);


    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);
    ResourcePack give{{ResourceType::Wood, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};

    player1.addResources(give);
    player2.addResources(receive);

    PlayerTradeRequestMove request(player1.getPlayerId(), give, receive);
    request.apply(session);

    TradeId tradeId = session.activeTrades().front()->id();

    PlayerTradeResponseMove response(player2.getPlayerId(), tradeId);
    response.apply(session);

    PlayerTradeAcceptMove accept(player1.getPlayerId(), player2.getPlayerId(), tradeId);

    REQUIRE(accept.isValid(session));
}
TEST_CASE("PlayerTradeAcceptMove apply exchanges resources and removes trade") {
    auto session = make2PGame();
    auto& player1 = session.player(0);
    auto& player2   = session.player(1);

    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    ResourcePack give{{ResourceType::Wheat, 1}};
    ResourcePack receive{{ResourceType::Ore, 1}};

    player1.addResources(give);
    player2.addResources(receive);

    PlayerTradeRequestMove request(player1.getPlayerId(), give, receive);
    request.apply(session);
    TradeId tradeId = session.activeTrades().front()->id();

    PlayerTradeResponseMove response(player2.getPlayerId(), tradeId);
    response.apply(session);

    PlayerTradeAcceptMove accept(player1.getPlayerId(), player2.getPlayerId(), tradeId);
    accept.apply(session);

    auto player1Resources = player1.getResources();
    auto player2Resources = player2.getResources();

    REQUIRE(player1Resources[ResourceType::Wheat] == 0);
    REQUIRE(player1Resources[ResourceType::Ore] == 1);

    REQUIRE(player2Resources[ResourceType::Ore] == 0);
    REQUIRE(player2Resources[ResourceType::Wheat] == 1);

    REQUIRE(session.activeTrades().empty());
}

TEST_CASE("PlayerTradeResponseMove is Valid only if the player has resources") {
    auto session = make2PGame();
    auto& player1 = session.player(0);
    auto& player2   = session.player(1);


    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    ResourcePack give{{ResourceType::Wood, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};

    player1.addResources(give);
    player2.addResources(receive);

    PlayerTradeRequestMove request(player1.getPlayerId(), give, receive);
    request.apply(session);
    TradeId tradeId = session.activeTrades().front()->id();

    PlayerTradeResponseMove response(player2.getPlayerId(), tradeId);
    REQUIRE(response.isValid(session));
}

TEST_CASE("PlayerTradeAcceptMove allValid returns responders") {
    auto session = make2PGame();
    auto& player1 = session.player(0);
    auto& player2   = session.player(1);


    forceCurrent(session, player1.getPlayerId());

    forcePhase(session, TurnPhase::Main);

    ResourcePack give{{ResourceType::Wood, 1}};
    ResourcePack receive{{ResourceType::Brick, 1}};

    player1.addResources(give);
    player2.addResources(receive);

    PlayerTradeRequestMove request(player1.getPlayerId(), give, receive);
    request.apply(session);
    TradeId tradeId = session.activeTrades().front()->id();

    PlayerTradeResponseMove response(player2.getPlayerId(), tradeId);
    response.apply(session);

    PlayerTradeAcceptMove accept(player1.getPlayerId(), player2.getPlayerId(), tradeId);
    auto valid = accept.allValid(session);

    REQUIRE(valid.count(1)>0);
    REQUIRE_FALSE(valid.count(0)>0);
}
