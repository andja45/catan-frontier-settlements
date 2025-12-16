#include "../../headers/Tile.h"

Tile::Tile() = default;

std::string Tile::typeToString(ResourceType type){
    {
        switch (type) {
        case ResourceType::Wood:
            return "Wood";
        case ResourceType::Brick:
            return "Brick";
        case ResourceType::Ore:
            return "Ore";
        case ResourceType::Wool:
            return "Wool";
        case ResourceType::Wheat:
            return "Wheat";
        case ResourceType::Desert:
            return "Desert";
        case ResourceType::Sea:
            return "Sea";
        default:
            return "Unknown";
        }
    }
}
