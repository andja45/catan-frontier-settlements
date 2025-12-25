#ifndef RESOURCETYPE_H
#define RESOURCETYPE_H

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
static const std::vector<ResourceType, 5> ResourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
    ResourceType::Ore,
	ResourceType::Wool,
	ResourceType::Wheat
}

#endif // RESOURCETYPE_H
