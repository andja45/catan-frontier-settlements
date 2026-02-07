#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
#include <algorithm>
#include <map>
#include <string>
#include <array>

enum class ResourceType {
    Wood,
    Brick,
    Ore,
    Wool,
    Wheat,
    None, // special type for ports and non-resource tiles
    Desert, // for desert tiles
    Sea, // for sea tiles
};
static const inline std::map<std::string,ResourceType> mapStringToRes = {
    {"none",ResourceType::None},
    {"wood",ResourceType::Wood},
    {"wool",ResourceType::Wool},
    {"brick",ResourceType::Brick},
    {"ore",ResourceType::Ore},
    {"desert",ResourceType::Desert},
    {"sea",ResourceType::Sea},
    {"wheat",ResourceType::Wheat}

};
static ResourceType fromString(const std::string& s) {
    std::string s2 = s;
    std::transform(s2.begin(), s2.end(), s2.begin(),
        [](unsigned char c){ return std::tolower(c); });
    if (mapStringToRes.find(s2) != mapStringToRes.end())
        return mapStringToRes.at(s);
    return ResourceType::None;
}

static bool resourceFromString(const std::string& s, ResourceType &outType) {
    auto it = mapStringToRes.find(s);
    if (it == mapStringToRes.end())
        return false;

    outType = it->second;
    return true;
}

inline std::string toString (ResourceType resource)
{
    switch (resource) {
    case ResourceType::Wood:  return "Wood";
    case ResourceType::Brick: return "Brick";
    case ResourceType::Wool: return "Wool";
    case ResourceType::Wheat: return "Wheat";
    case ResourceType::Ore:   return "Ore";
    case ResourceType::Sea:  return "Sea";
    case ResourceType::Desert:  return "Desert";
    default: return "None";
    }
}

static const std::array<ResourceType, 5> ResourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
	ResourceType::Wheat,
	ResourceType::Wool,
    ResourceType::Ore,
};

#endif // RESOURCETYPE_H
