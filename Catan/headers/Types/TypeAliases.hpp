//
// Created by matija on 12/13/25.
//

#ifndef Catan_TYPEALIASES_HPP
#define Catan_TYPEALIASES_HPP
#include <array>
#include <utility>

class Edge;
class Node;
class Tile;
using HexCoords      = std::pair<int,int> ;
using NeighbourTiles = std::array<Tile*,6>;
using NeighbourNodes = std::array<Node*,6>;
using NeighbourEdges = std::array<Edge*,6>;

using IncidentEdges = std::array<Edge*,3>;
using IncidentTiles = std::array<Tile*,3>;
#endif //Catan_TYPEALIASES_HPP