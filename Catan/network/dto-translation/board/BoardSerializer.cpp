//
// Created by matija on 1/24/26.
//

#include "BoardSerializer.hpp"

catan::ResourceType toProto(ResourceType r) {
    return static_cast<catan::ResourceType>(r);
}