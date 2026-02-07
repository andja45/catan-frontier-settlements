//
// Created by matija on 1/5/26.
//

#ifndef CATAN_LOADFROMFILECREATOR_HPP
#define CATAN_LOADFROMFILECREATOR_HPP
#include <board/factory/AbstractBoardCreator.hpp>

class LoadFromFileCreator : public AbstractBoardCreator {
private:
    std::string m_filePath;

    std::pair<std::vector<TileDef>, std::vector<PortDef>> loadBoardFromFile(); // loads from file based on extension
    static std::pair<std::vector<TileDef>, std::vector<PortDef>> loadBoardFromTxt(std::istream &stream);
    static std::pair<std::vector<TileDef>, std::vector<PortDef>> loadBoardFromJson(std::istream &stream);
public:
    std::unique_ptr<Board> getBoard() override;

    LoadFromFileCreator(const std::string& filePath) : m_filePath(filePath) {}
};

#endif //CATAN_LOADFROMFILECREATOR_HPP