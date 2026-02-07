//
// Created by matija on 12/13/25.
//

#include <array>
#include <string>

#ifndef Catan_DEVTYPE_HPP
#define Catan_DEVTYPE_HPP

enum class DevCardType {
    None,
    Monopoly,
    Knight,
    RoadBuilding,
    YearOfPlenty,
    VictoryPoint
};

static const std::array<DevCardType, 5> DevCardTypes = {
    DevCardType::Monopoly,
    DevCardType::Knight,
    DevCardType::RoadBuilding,
    DevCardType::YearOfPlenty,
    DevCardType::VictoryPoint
};

inline std::string toString(DevCardType cardType) {
    switch (cardType) {
        case DevCardType::Monopoly:     return "Monopoly";
        case DevCardType::Knight:       return "Knight";
        case DevCardType::RoadBuilding: return "RoadBuilding";
        case DevCardType::YearOfPlenty: return "YearOfPlenty";
        case DevCardType::VictoryPoint: return "VictoryPoint";
        default:                        return "None";
    }
}

#endif //Catan_DEVTYPE_HPP
