//
// Created by jovan on 24/01/2026.
//
#include "MessageFramer.h"

void MessageFramer::setMessageCallback(MessageCallback cb) {
    callback = std::move(cb);
}

QByteArray MessageFramer::frameMessage(const QByteArray& payload) {
    QByteArray framed;
    uint32_t size = payload.size();

    framed.append(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
    framed.append(payload);

    return framed;
}

void MessageFramer::onBytes(const QByteArray& bytes) {
    buffer.append(bytes);

    while (true) {
        if (expectedSize == 0) {
            if (buffer.size() < 4)
                return;

            expectedSize =
              *reinterpret_cast<const uint32_t*>(buffer.constData());

            buffer.remove(0, 4);
        }

        if (buffer.size() < expectedSize)
            return;

        QByteArray message = buffer.left(expectedSize);
        buffer.remove(0, expectedSize);
        expectedSize = 0;

        if (callback)
            callback(message);
    }
}
