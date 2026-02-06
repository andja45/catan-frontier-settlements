//
// Created by andja on 6.2.26.
//
#include <catch2/catch_test_macros.hpp>
#include "TestHelper.h"
#include "move/devcard/PlayDevCardMove.h"

static void giveDevCard(GameSession& session, PlayerId p, DevCardType card) {
    session.player(p).addDevCard(card);
}

TEST_CASE("PlayDevCardMove::isValid", "[PlayDevCardMove][isValid]") {
    GameSession session = make2PGame();
    const PlayerId p0 = session.currentPlayer();
    const PlayerId p1 = 1;

    SECTION("Not current player -> false") {
        forcePhase(session, TurnPhase::Main); // needed to skip initialplacement
        giveDevCard(session, p1, DevCardType::Knight);

        PlayDevCardMove move(p1, DevCardType::Knight);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Wrong phase -> false") {
        giveDevCard(session, p0, DevCardType::Knight);

        PlayDevCardMove move(p0, DevCardType::Knight);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Player does not have that dev card -> false") {
        forcePhase(session, TurnPhase::Main);

        PlayDevCardMove move(p0, DevCardType::Monopoly);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("Already played a dev card this turn -> false") {
        forcePhase(session, TurnPhase::Main);
        giveDevCard(session, p0, DevCardType::Knight);

        session.markDevCardPlayedThisTurn();

        PlayDevCardMove move(p0, DevCardType::Knight);
        REQUIRE(move.isValid(session) == false);
    }

    SECTION("All requirements satisfied -> true") {
        forcePhase(session, TurnPhase::Main);
        giveDevCard(session, p0, DevCardType::Knight);

        PlayDevCardMove move(p0, DevCardType::Knight);
        REQUIRE(move.isValid(session) == true);
    }
}

TEST_CASE("PlayDevCardMove::apply removes card, marks played, and sets correct phase",
          "[PlayDevCardMove][apply]") {
    GameSession session = make2PGame();
    const PlayerId p0 = session.currentPlayer();

    forcePhase(session, TurnPhase::Main);
    giveDevCard(session, p0, DevCardType::Knight);

    const int knightsBefore = session.player(p0).getKnightsUsed();

    PlayDevCardMove move(p0, DevCardType::Knight);
    REQUIRE(move.isValid(session) == true);

    move.apply(session);

    REQUIRE(session.player(p0).hasDevCard(DevCardType::Knight) == false);
    REQUIRE(session.hasPlayedDevCardThisTurn() == true);
    REQUIRE(session.phase() == TurnPhase::SetRobber);

    REQUIRE(session.player(p0).getKnightsUsed() == knightsBefore + 1);
}

TEST_CASE("PlayDevCardMove sets correct phase for each card type", "[PlayDevCardMove][phase]") {
    struct Case { DevCardType card; TurnPhase expected; };

    const Case cases[] = {
        {DevCardType::Knight,        TurnPhase::SetRobber},
        {DevCardType::RoadBuilding,  TurnPhase::RoadBuilding},
        {DevCardType::YearOfPlenty,  TurnPhase::YearOfPlenty},
        {DevCardType::Monopoly,      TurnPhase::Monopoly},
    };

    for (auto c : cases) {
        GameSession session = make2PGame();
        const PlayerId p0 = session.currentPlayer();

        forcePhase(session, TurnPhase::Main);
        giveDevCard(session, p0, c.card);

        PlayDevCardMove move(p0, c.card);
        REQUIRE(move.isValid(session) == true);

        move.apply(session);

        REQUIRE(session.phase() == c.expected);
        REQUIRE(session.hasPlayedDevCardThisTurn() == true);
        REQUIRE(session.player(p0).hasDevCard(c.card) == false);
    }
}
