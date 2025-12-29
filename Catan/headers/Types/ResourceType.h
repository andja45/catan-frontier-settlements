#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H
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
static const std::array<ResourceType, 5> ResourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
    ResourceType::Ore,
	ResourceType::Wool,
	ResourceType::Wheat
}

#endif // RESOURCETYPE_H
