//
// Created by matija on 1/4/26.
//

#ifndef CATAN_BOARDLAYOUTTOJSON_HPP
#define CATAN_BOARDLAYOUTTOJSON_HPP
#include <board/serialization/AbstractBoardSerializer.hpp>

class BoardLayoutToJson :public AbstractBoardSerializer{
public:
    explicit BoardLayoutToJson(const std::string &path)
    : AbstractBoardSerializer(path) {
    }

    void serialize(const Board &board) override;

    ~BoardLayoutToJson() override{}
};

#endif //CATAN_BOARDLAYOUTTOJSON_HPP