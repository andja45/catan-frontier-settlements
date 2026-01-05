//
// Created by matija on 12/13/25.
//

#ifndef Catan_TYPEALIASES_HPP
#define Catan_TYPEALIASES_HPP
#include <array>
#include <map>
#include <utility>
#include <board/Coords/AxialCoords.hpp>
#include <types/Directions.h>

#include "ResourceType.h"

class Edge;
class Node;
class Tile;

using TileCoords      = AxialCoords ;

using NeighbourTiles = std::vector<Tile*>; // TODO class with iterator, keep in board instead of tile?
using NeighbourNodes = std::vector<Node*>;
using NeighbourEdges = std::vector<Edge*>;

using IncidentEdges = std::vector<Edge*>;
using IncidentTiles = std::vector<Tile*>;

using ResourcePack = std::map<ResourceType, int>;

using PlayerId = int;
using NodeId = int;
using EdgeId = int;
using TileId = int;
using EdgeIndex = int;
using NodeIndex = int;

using EdgeDirection = SideDirection;
using NodeDirection = PointDirection;
using TileDirection = SideDirection;

#endif //Catan_TYPEALIASES_HPP