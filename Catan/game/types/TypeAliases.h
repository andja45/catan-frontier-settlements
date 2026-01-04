//
// Created by matija on 12/13/25.
//

#ifndef Catan_TYPEALIASES_HPP
#define Catan_TYPEALIASES_HPP
#include <array>
#include <map>
#include <utility>
#include "ResourceType.h"

class Edge;
class Node;
class Tile;

using HexCoords      = std::pair<int,int> ;

using NeighbourTiles = std::array<Tile*,6>;
using NeighbourNodes = std::array<Node*,6>;
using NeighbourEdges = std::array<Edge*,6>;

using IncidentEdges = std::array<Edge*,3>;
using IncidentTiles = std::array<Tile*,3>;

using ResourcePack = std::map<ResourceType, int>;

using PlayerId = int; // TODO dodati za sve sto ima id
using NodeId = int;
using EdgeId = int;
using TileId = int;
using EdgeIndex = int;
using NodeIndex = int;

#endif //Catan_TYPEALIASES_HPP