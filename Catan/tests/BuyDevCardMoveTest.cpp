//
// Created by andja on 6.2.26..
//
#include <catch2/catch_test_macros.hpp>
#include "TestHelper.h"
#include "move/devcard/BuyDevCardMove.h"

static void emptyBankDevs(GameSession& session) {
    Bank& bank = session.bank();
    while (bank.hasDevCards()) {
        (void)bank.takeRandomDev();
    }
    REQUIRE(bank.hasDevCards() == false);
}

TEST_CASE("BuyDevCardMove::isValid", "[BuyDevCardMove][isValid]") {
    GameSession session = make2PGame();
    const PlayerId p0 = 0;
    const PlayerId p1 = 1;

    SECTION("Not current player -> false") {
        forcePhase(session, TurnPhase::Main); // needed to skip initialplacement
        giveResources(session, p1, Costs::DevCard);

        BuyDevCardMove move(p1);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Wrong phase -> false") {
        // default RollDice at game start
        REQUIRE(session.phase() != TurnPhase::Main);

        giveResources(session, p0, Costs::DevCard);
        BuyDevCardMove move(p0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Bank has no dev cards -> false") {
        forcePhase(session, TurnPhase::Main);
        giveResources(session, p0, Costs::DevCard);

        emptyBankDevs(session);

        BuyDevCardMove move(p0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Player lacks resources -> false") {
        forcePhase(session, TurnPhase::Main);
        // give nothing
        BuyDevCardMove move(p0);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("All requirements satisfied -> true") {
        forcePhase(session, TurnPhase::Main);
        giveResources(session, p0, Costs::DevCard);

        REQUIRE(session.bank().hasDevCards() == true);

        BuyDevCardMove move(p0);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("BuyDevCardMove::apply effects", "[BuyDevCardMove][apply]") {
    GameSession session = make2PGame();
    const PlayerId p0 = 0;

    forcePhase(session, TurnPhase::Main);
    REQUIRE(session.bank().hasDevCards() == true);

    giveResources(session, p0, Costs::DevCard);

    Player& player = session.player(p0);

    const int cardsBefore   = player.getNumOfResourceCards();
    const int pointsBefore  = player.getTotalPoints();
    const int devsBefore    = player.getNumOfDevCards();

    BuyDevCardMove move(p0);
    REQUIRE(move.isValid(session) == true);

    REQUIRE(session.applyMove(move) == true);

    const int cardsAfter  = player.getNumOfResourceCards();
    const int pointsAfter = player.getTotalPoints();
    const int devsAfter   = player.getNumOfDevCards();

    // resources were paid
    REQUIRE(cardsAfter == cardsBefore - 3);

    // player got exactly 1 dev card
    REQUIRE(devsAfter == devsBefore + 1);

    // points either stay same or +1 (only if VictoryPoint card was drawn)
    REQUIRE((pointsAfter == pointsBefore || pointsAfter == pointsBefore + 1));
}
