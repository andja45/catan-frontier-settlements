//
// Created by matija on 12/13/25.
//

#ifndef Catan_DEVTYPE_HPP
#define Catan_DEVTYPE_HPP
enum class DevType {
    None,
    Monopoly,
    Knight,
    RoadBuilding,
    Resources,
    VictoryPoint
};
static const std::vector<DevType, 5> DevCardTypes = {
    DevType::Monopoly,
    DevType::Knight,
    DevType::RoadBuilding,
	DevType::Resources,
	DevType::VictoryPoint
}
#endif //Catan_DEVTYPE_HPP