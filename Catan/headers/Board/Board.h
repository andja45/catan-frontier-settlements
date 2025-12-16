//
// Created by andja on 10.12.25..
//

#ifndef Catan_BOARD_H
#define Catan_BOARD_H

#include <map>
#include <memory>

#include "Edge.h"
#include "Tile.h"
#include "Node.h"
#include "../../headers/Types/TypeAliases.hpp"

class Board {
private:
    std::map<HexCoords,std::unique_ptr<Tile>> _tiles;
    std::vector<std::unique_ptr<Node>> _nodes;
    std::vector<std::unique_ptr<Edge>> _edges;
    
    std::map<int, std::vector<Tile*>> _tilesByNumber;
public:
    std::vector<Tile*> getTilesWithNumber(int num);
    Tile* getTileAt(HexCoords coords);
    Node* getNodeAt(HexCoords coords, int index);
    Edge* getEdgeAt(HexCoords coords, int index);


    static void standardizeCoords(HexCoords& coords, int& index);

    void initializeStandardBoard();

};


#endif //Catan_BOARD_H
