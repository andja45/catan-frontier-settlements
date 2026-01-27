//
// Created by jovan on 24/01/2026.
//

#ifndef CATAN_MESSAGEFRAMER_H
#define CATAN_MESSAGEFRAMER_H

#pragma once
#include <QByteArray>
#include <functional>
#include <cstdint>

class MessageFramer {
public:
    using MessageCallback = std::function<void(const QByteArray&)>;

    void onBytes(const QByteArray& bytes);
    void setMessageCallback(MessageCallback cb);

    static QByteArray frameMessage(const QByteArray& payload);

private:
    QByteArray buffer;
    uint32_t expectedSize = 0;
    MessageCallback callback;
};


#endif //CATAN_MESSAGEFRAMER_H