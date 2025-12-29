#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
#include <map>
#include <string>

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
static ResourceType fromString(const std::string& s) {
        return mapStringToRes.at(s);
    }


#endif // RESOURCETYPE_H
