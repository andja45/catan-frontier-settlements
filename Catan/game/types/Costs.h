//
// Created by andja on 17.12.25..
//

#ifndef CATAN_COSTS_H
#define CATAN_COSTS_H

#include "ResourceType.h"
#include  "../types/TypeAliases.h"

namespace Costs {
        inline const ResourcePack Road {
                    {ResourceType::Brick, 1},
                    {ResourceType::Wood, 1}
        };

        inline const ResourcePack Settlement {
                    {ResourceType::Brick, 1},
                    {ResourceType::Wood, 1},
                    {ResourceType::Wool, 1},
                    {ResourceType::Wheat, 1}
        };

        inline const ResourcePack City {
                    {ResourceType::Ore, 3},
                    {ResourceType::Wheat, 2}
        };

        inline const ResourcePack DevCard {
                    {ResourceType::Ore, 1},
                    {ResourceType::Wool, 1},
                    {ResourceType::Wheat, 1}
        };
}

#endif //CATAN_COSTS_H