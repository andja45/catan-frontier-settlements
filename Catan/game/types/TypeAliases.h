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

using HexCoords      = AxialCoords ;

using NeighbourTiles = std::array<Tile*,6>; // TODO class with iterator, keep in board instead of tile?
using NeighbourNodes = std::array<Node*,6>;
using NeighbourEdges = std::array<Edge*,6>;

using IncidentEdges = std::array<Edge*,3>;
using IncidentTiles = std::array<Tile*,3>;

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