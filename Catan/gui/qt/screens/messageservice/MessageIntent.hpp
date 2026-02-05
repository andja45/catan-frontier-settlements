//
// Created by matija on 2/3/26.
//

#ifndef CATAN_MESSAGEINTENT_HPP
#define CATAN_MESSAGEINTENT_HPP
#include <string>

enum class MessageType {
    Error,
    Info,
    Warning,
    None
};

struct MessageIntent {
    MessageType type;
    std::string message;
    bool isEmpty() {return type == MessageType::None;}
};
#endif //CATAN_MESSAGEINTENT_HPP