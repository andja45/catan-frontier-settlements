//
// Created by matija on 1/24/26.
//

#ifndef CATAN_BOARDSERIALIZER_HPP
#define CATAN_BOARDSERIALIZER_HPP


class BoardSerializer {
public:
     static catan::ResourceType toProto(const Board& board);
};

#endif //CATAN_BOARDSERIALIZER_HPP