//
// Created by matija on 1/4/26.
//

#ifndef CATAN_BOARDLAYOUTTOJSON_HPP
#define CATAN_BOARDLAYOUTTOJSON_HPP
#include <board/BoardSerializers/AbstractBoardSerializer.hpp>

class BoardLayoutToJson :public AbstractBoardSerializer{
public:
    void serialize(Board) override;
};

#endif //CATAN_BOARDLAYOUTTOJSON_HPP