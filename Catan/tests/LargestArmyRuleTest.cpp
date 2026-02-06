#include <catch2/catch_test_macros.hpp>
#include "rules/LargestArmyRule.h"
#include "model/GameSession.h"
#include "board/Board.h"
#include "player/Player.h"


static GameSession makeTestSession() {
    std::vector<std::string> names = {"Ana", "Marko"};
    auto board = std::make_unique<Board>();
    board->initializeBoard({TileDef{0, 0, ResourceType::Wheat, 8}});
    return GameSession(names, 0, 1, std::move(board), 10, "test");
}

TEST_CASE("LargestArmyRule - Basic Logic", "[rule][army]") {
    GameSession session = makeTestSession();
    LargestArmyRule armyRule;


    PlayerId p0 = session.players()[0]->getPlayerId();
    PlayerId p1 = session.players()[1]->getPlayerId();

    SECTION("Initial state - No one has the title") {
        armyRule.evaluate(session);
        REQUIRE(session.largestArmyOwner() == types::InvalidPlayerId);
    }

    SECTION("Player reaches 3 knights - Gets the title") {
        for (int i = 0; i < 3; i++) {
            session.player(p0).addKnight();
        }
        armyRule.evaluate(session);

        REQUIRE(session.largestArmyOwner() == p0);
    }

    SECTION("Title defense - Must have strictly more knights") {

        for (int i = 0; i < 3; i++) {
            session.player(p0).addKnight();
        }
        armyRule.evaluate(session);
        REQUIRE(session.largestArmyOwner() == p0);

        for (int i = 0; i < 3; i++) {
            session.player(p1).addKnight();
        }
        armyRule.evaluate(session);
        REQUIRE(session.largestArmyOwner() == p0);

        for (int i = 0; i < 4; i++) {
            session.player(p1).addKnight();
        }
        armyRule.evaluate(session);
        REQUIRE(session.largestArmyOwner() == p1);
    }

    SECTION("Minimum requirement is 3 knights") {
        for (int i = 0; i < 2; i++) {
            session.player(p0).addKnight();
        }
        armyRule.evaluate(session);

        REQUIRE(session.largestArmyOwner() == types::InvalidPlayerId);
    }
}