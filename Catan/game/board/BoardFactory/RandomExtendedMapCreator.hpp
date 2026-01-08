//
// Created by matija on 1/5/26.
//

#ifndef CATAN_RANDOMSTANDARSEXTENDEDCREATOR_HPP
#define CATAN_RANDOMSTANDARSEXTENDEDCREATOR_HPP
#include <board/Board.h>
#include <board/BoardFactory/AbstractBoardCreator.hpp>

// generates extended catan map version for 6 players
class RandomExtendedMapCreator : public AbstractBoardCreator{
private:
    static std::vector<TileDef> generateRandomBoard();
    static void generatePorts(Board*);
public:
    std::unique_ptr<Board> getBoard() override;

};

#endif //CATAN_RANDOMSTANDARSEXTENDEDCREATOR_HPP