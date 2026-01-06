//
// Created by matija on 1/4/26.
//

#ifndef CATAN_BOARDCATALOG_HPP
#define CATAN_BOARDCATALOG_HPP
#include <string>
#include <unordered_map>
#include <board/BoardFactory/AbstractBoardCreator.hpp>

// TODO not used
// intended for usage as simple factory method with dynamic registration of board creators
class BoardCatalog {
private:
    std::unordered_map<std::string, AbstractBoardCreator*> m_creators;
public:
    void addCreator(const std::string &name, AbstractBoardCreator* creator);
    std::unique_ptr<Board> getBoard(const std::string &name);
};

#endif //CATAN_BOARDCATALOG_HPP