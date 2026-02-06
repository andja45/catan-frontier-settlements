//
// Created by marko-cvijetinovic on 2/5/26.
//

#include <catch2/catch_test_macros.hpp>

#include <player/Bank.h>
#include <types/ResourceType.h>

#include <map>

static int sumDevCards(const std::map<DevCardType,int>& m) {
    int s = 0;
    for (auto const& [t,c] : m) s += c;
    return s;
}

static int sumResources(const std::map<ResourceType,int>& m) {
    int s = 0;
    for (auto const& [t,c] : m) s += c;
    return s;
}

TEST_CASE("Default Bank initializes with standard resources and dev cards")
{
    Bank b;

    REQUIRE(b.getName() == "Bank");
    REQUIRE(b.hasLongestRoad());
    REQUIRE(b.hasLargestMilitary());

    // Resources: should contain the 5 base types, each with standard amount.
    // We can assert they are >= 0 and present.
    REQUIRE(b.hasResource(ResourceType::Wood, 1));
    REQUIRE(b.hasResource(ResourceType::Brick, 1));
    REQUIRE(b.hasResource(ResourceType::Wool, 1));
    REQUIRE(b.hasResource(ResourceType::Wheat, 1));
    REQUIRE(b.hasResource(ResourceType::Ore, 1));

    // Dev cards: should match standard totals (5+2+2+2+14 = 25)
    REQUIRE(b.getNumOfDevCards() == 25);

    REQUIRE(b.hasDevCard(DevCardType::VictoryPoint));
    REQUIRE(b.hasDevCard(DevCardType::Monopoly));
    REQUIRE(b.hasDevCard(DevCardType::RoadBuilding));
    REQUIRE(b.hasDevCard(DevCardType::YearOfPlenty));
    REQUIRE(b.hasDevCard(DevCardType::Knight));
}

TEST_CASE("Custom Bank initializes exactly with provided resources/dev cards")
{
    ResourcePack res {
        {ResourceType::Wood, 3},
        {ResourceType::Brick, 4},
        {ResourceType::Wool,  0},
        {ResourceType::Wheat, 1},
        {ResourceType::Ore,   2},
    };

    DevPack dev {
        {DevCardType::Knight, 2},
        {DevCardType::Monopoly, 1},
    };

    Bank b(res, dev);

    REQUIRE(b.getName() == "Bank");
    REQUIRE(b.hasLongestRoad());
    REQUIRE(b.hasLargestMilitary());

    REQUIRE(b.getNumOfResourceCards() == 3+4+0+1+2);

    REQUIRE(b.getNumOfResourceCards(ResourceType::Wood)  == 3);
    REQUIRE(b.getNumOfResourceCards(ResourceType::Brick) == 4);
    REQUIRE(b.getNumOfResourceCards(ResourceType::Wool)  == 0);
    REQUIRE(b.getNumOfResourceCards(ResourceType::Wheat) == 1);
    REQUIRE(b.getNumOfResourceCards(ResourceType::Ore)   == 2);

    REQUIRE(b.getNumOfDevCards() == 3);
    REQUIRE(b.getNumOfDevCards(DevCardType::Knight) == 2);
    REQUIRE(b.getNumOfDevCards(DevCardType::Monopoly) == 1);
}

TEST_CASE("takeRandomDev decreases dev card count")
{
    // Controlled dev pack to make sure multiple keys exist and are > 0
    ResourcePack res {
            {ResourceType::Wood, 1},
            {ResourceType::Brick, 1},
            {ResourceType::Wool, 1},
            {ResourceType::Wheat, 1},
            {ResourceType::Ore, 1},
        };

    DevPack dev {
            {DevCardType::Knight, 2},
            {DevCardType::Monopoly, 1},
            {DevCardType::VictoryPoint, 1},
        };

    Bank b(res, dev);

    auto before = b.getDevCards();
    int totalBefore = b.getNumOfDevCards();
    REQUIRE(totalBefore == 4);

    DevCardType drawn = b.takeRandomDev();

    auto after = b.getDevCards();
    int totalAfter = b.getNumOfDevCards();
    REQUIRE(totalAfter == totalBefore - 1);

    // Drawn type must have existed with >0 and decrease by 1
    REQUIRE(before.count(drawn) > 0);
    REQUIRE(before.at(drawn) > 0);
    REQUIRE(after[drawn] == before.at(drawn) - 1);

    // All other types unchanged
    for (auto const& [t,c] : before) {
        if (t == drawn) continue;
        REQUIRE(after[t] == c);
    }
}

TEST_CASE("takeRandomDev can exhaust a bank (invariant test)")
{
    ResourcePack res {
            {ResourceType::Wood, 1},
            {ResourceType::Brick, 1},
            {ResourceType::Wool, 1},
            {ResourceType::Wheat, 1},
            {ResourceType::Ore, 1},
        };

    DevPack dev {
            {DevCardType::Knight, 3},
            {DevCardType::Monopoly, 2},
        };

    Bank b(res, dev);
    REQUIRE(b.getNumOfDevCards() == 5);

    // Draw exactly all dev cards
    for (int i = 0; i < 5; ++i) {
        (void)b.takeRandomDev();
    }
    REQUIRE(b.getNumOfDevCards() == 0);
}
