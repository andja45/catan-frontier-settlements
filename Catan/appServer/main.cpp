//
// Created by matija on 2/2/26.
//

#include <qcoreapplication.h>
#include <ServerController.hpp>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    std::unique_ptr<ServerController> controller = std::make_unique<ServerController>();

    constexpr quint16 port = 3460;
    controller->listen(port);

    qDebug() << "[Server] Event loop started";

    return app.exec();
}
