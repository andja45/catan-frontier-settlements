#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
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
    {"None",ResourceType::None},
    {"Wood",ResourceType::Wood},
    {"Wool",ResourceType::Wool},
    {"Brick",ResourceType::Brick},
    {"Ore",ResourceType::Ore},
    {"Desert",ResourceType::Desert},
    {"Sea",ResourceType::Sea},
    {"Wheat",ResourceType::Wheat}

};
static ResourceType fromString(const std::string& s) {
        return mapStringToRes.at(s);
    }

static const std::array<ResourceType, 5> ResourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
    ResourceType::Ore,
	ResourceType::Wool,
	ResourceType::Wheat
};

#endif // RESOURCETYPE_H
