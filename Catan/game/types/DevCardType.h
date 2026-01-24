//
// Created by matija on 12/13/25.
//

#include <array>

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
#endif //Catan_DEVTYPE_HPP
