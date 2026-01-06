//
// Created by matija on 12/13/25.
//

#ifndef Catan_TYPEALIASES_HPP
#define Catan_TYPEALIASES_HPP
#include <map>
#include <vector>

enum class DevType;
class Edge;
class Node;
class Tile;
class AxialCoords;
enum class ResourceType;
enum class SideDirection;
enum class PointDirection;

using TileCoords = AxialCoords ;

using NeighbourTiles = std::vector<Tile*>; // TODO class with iterator, keep in board instead of tile?
using NeighbourNodes = std::vector<Node*>;
using NeighbourEdges = std::vector<Edge*>;

using IncidentEdges = std::vector<Edge*>;
using IncidentTiles = std::vector<Tile*>;

using ResourcePack = std::map<ResourceType, int>;
using DevPack = std::map<DevType,int>;

using PlayerId = int;
using NodeId = int;
using EdgeId = int;
using TileId = int;
using EdgeIndex = int;
using NodeIndex = int;

using EdgeDirection = SideDirection;
using NodeDirection = PointDirection;
using TileDirection = SideDirection;

using TileType = ResourceType;
using TradeType = ResourceType;
using ResourceCardType = ResourceType;

#endif //Catan_TYPEALIASES_HPP