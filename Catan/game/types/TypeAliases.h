//
// Created by matija on 12/13/25.
//

#ifndef Catan_TYPEALIASES_HPP
#define Catan_TYPEALIASES_HPP
#include <map>
#include <vector>
#include <types/Directions.h>

enum class DevType;
class Edge;
class Node;
class Tile;
class AxialCoords;
class Move;
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
using Tool = Move;

namespace types {
    constexpr PlayerId InvalidPlayer = -1;
    constexpr NodeId InvalidNode = -1;
    constexpr EdgeId InvalidEdge = -1;
    constexpr TileId InvalidTile = -1;
    constexpr Tool* InvalidTool = nullptr;
}

using EdgeDirection = SideDirection;
using NodeDirection = PointDirection;
using TileDirection = SideDirection;

using EdgeAsciiDirection = PointDirection;
using NodeAsciiDirection = SideDirection;
using TileAsciiDirection = PointDirection;

using TileType = ResourceType;
using TradeType = ResourceType;
using ResourceCardType = ResourceType;

namespace types {


}


#endif //Catan_TYPEALIASES_HPP