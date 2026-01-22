#include "MoveCosts.h"

std::vector<ResourceType> MoveCosts::costFor(ToolbarActionType action)
{
    switch (action) {
    case ToolbarActionType::BuildRoad:
        return {
            ResourceType::Wood,
            ResourceType::Brick
        };

    case ToolbarActionType::BuildSettlement:
        return {
            ResourceType::Wood,
            ResourceType::Brick,
            ResourceType::Wheat,
            ResourceType::Wool
        };

    case ToolbarActionType::BuildCity:
        return {
            ResourceType::Wheat,
            ResourceType::Wheat,
            ResourceType::Ore,
            ResourceType::Ore,
            ResourceType::Ore
        };

    case ToolbarActionType::BuyDevCard:
        return {
            ResourceType::Wheat,
            ResourceType::Wool,
            ResourceType::Ore
        };

    default:
        return {};
    }
}
