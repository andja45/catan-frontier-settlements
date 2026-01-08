//
// Created by matija on 1/4/26.
//

#ifndef CATAN_ABSTRACTBOARDSERIALIZER_HPP
#define CATAN_ABSTRACTBOARDSERIALIZER_HPP
#include <string>
#include <board/Board.h>

// abstraction for saving board
class AbstractBoardSerializer {
protected:
    std::string m_filePath;
public:
    virtual void serialize(Board)=0;
    virtual ~AbstractBoardSerializer() = default;
    AbstractBoardSerializer(std::string path) : m_filePath(path){}
};

#endif //CATAN_ABSTRACTBOARDSERIALIZER_HPP