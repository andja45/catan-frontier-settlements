#ifndef MOVECOSTS_H
#define MOVECOSTS_H

#include "../types/ToolbarActionType.h"
#include "../types/ResourceType.h"
#include <vector>

class MoveCosts {
public:
    MoveCosts();
    // Returns the cost of performing a toolbar action
    static std::vector<ResourceType> costFor(ToolbarActionType action);
};

#endif // MOVECOSTS_H
