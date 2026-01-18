//
// Created by matija on 1/4/26.
//

#ifndef CATAN_ABSTRACTBOARDCREATOR_HPP
#define CATAN_ABSTRACTBOARDCREATOR_HPP
#include <board/Board.h>

// abstraction for different methods of board loading
class AbstractBoardCreator {
public:
    virtual std::unique_ptr<Board> getBoard()=0;
    virtual ~AbstractBoardCreator() = default;
};

#endif //CATAN_ABSTRACTBOARDCREATOR_HPP