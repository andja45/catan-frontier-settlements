#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
#include <map>
#include <string>
#include <array>

enum class ResourceType {
    None,
    Wood,
    Brick,
    Ore,
    Wool,
    Desert,
    Sea,
    Wheat
};
static const inline std::map<std::string,ResourceType> mapStringToRes = {
    {"None",ResourceType::None},
    {"Wood",ResourceType::Wood},
    {"Wool",ResourceType::Wool},
    {"Brick",ResourceType::Brick},
    {"Ore",ResourceType::Ore},
    {"Desert",ResourceType::Desert},
    {"Sea",ResourceType::Sea},
    {"Wheat",ResourceType::Wheat}

};
static ResourceType resourceFromString(const std::string& s) {
    auto it = mapStringToRes.find(s);
    if (it == mapStringToRes.end()) {
        return ResourceType::None;
    }

    return it->second;
    //return mapStringToRes.at(s);
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
    ResourceType::Ore,
	ResourceType::Wool,
	ResourceType::Wheat
};

#endif // RESOURCETYPE_H
