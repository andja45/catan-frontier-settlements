#include "Tile.h"
#include <ostream>

std::string Tile::typeToString(ResourceType type){
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

std::string Tile::toString() const{
    if(m_type == ResourceType::Sea) return "Sea";
    return typeToString(m_type) + " " + std::to_string(m_number) +
           " (" + std::to_string(std::get<0>(m_tileCoord)) + ", " + std::to_string(std::get<1>(m_tileCoord)) +  ")";
}

std::ostream& operator<<(std::ostream& os, const Tile& tile)
{
    if (tile.m_type == ResourceType::Sea)
        return os << "Sea";

    return os << Tile::typeToString(tile.m_type) << " " << tile.m_number
              << " (" << std::get<0>(tile.m_tileCoord) << ", "
              << std::get<1>(tile.m_tileCoord) <<  ")";
}
