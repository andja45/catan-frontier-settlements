#ifndef MAPTYPE_H
#define MAPTYPE_H
#include <map>
#include <string>
#include <array>

enum class MapType {
    Classic,
    Extended,
    Custom
};
static const inline std::map<std::string,MapType> mapStringToMap = {
    {"Classic",MapType::Classic},
    {"Extended",MapType::Extended},
    {"Custom",MapType::Custom}

};

inline std::string toString (MapType type)
{
    switch (type) {
    case MapType::Classic:   return "Classic";
    case MapType::Extended:  return "Extended";
    case MapType::Custom:  return "Custom";
    }
}
static MapType mapFromString(const std::string& s) {
    return mapStringToMap.at(s);
}
static const std::array<MapType, 3> MapTypes = {MapType::Classic,MapType::Extended,MapType::Custom};
#endif // MAPTYPE_H
