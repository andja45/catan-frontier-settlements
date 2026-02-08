#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
#include <algorithm>
#include <map>
#include <string>
#include <array>
#include <cctype>

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

static ResourceType fromString(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

    if (auto it = mapStringToRes.find(s); it != mapStringToRes.end()) {
        return it->second;
    }

    return ResourceType::None;
}


static bool resourceFromString(const std::string& s, ResourceType &outType) {
    outType=fromString(s);
    return outType != ResourceType::None;
}

inline std::string toString (const ResourceType resource)
{
    switch (resource) {
    case ResourceType::Wood:  return "wood";
    case ResourceType::Brick: return "brick";
    case ResourceType::Wool: return "wool";
    case ResourceType::Wheat: return "wheat";
    case ResourceType::Ore:   return "ore";
    case ResourceType::Sea:  return "sea";
    case ResourceType::Desert:  return "desert";
    default: return "none";
    }
}

static constexpr std::array<ResourceType, 5> resourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
	ResourceType::Wheat,
	ResourceType::Wool,
    ResourceType::Ore,
};

#endif // RESOURCETYPE_H
