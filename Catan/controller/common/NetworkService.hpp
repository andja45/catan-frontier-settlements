//
// Created by matija on 2/2/26.
//

#ifndef CATAN_NETWORKSERVICE_HPP
#define CATAN_NETWORKSERVICE_HPP
#include <NetworkTransport.h>
#include <QSettings>
#include <common/PathService.hpp>

class NetworkService {
private:
    NetworkTransport* m_transport;

public:
    NetworkService() = default;
    NetworkTransport* getTransport() {
        if (m_transport == nullptr) {
            QString path = PathService::instance().resourceFile("client.ini");

            if (!QFile::exists(path)) {
                QFile::copy(":/config/client.ini", path);
            }

            QSettings settings(path, QSettings::IniFormat);

            QString host = settings.value("network/host", "127.0.0.1").toString();
            quint16 port = settings.value("network/port", 3460).toUInt();

            m_transport = new NetworkTransport(nullptr);
            QTcpSocket* socket = new QTcpSocket(m_transport);
            m_transport->setSocket(socket);
            socket->connectToHost(host, port);

            //for debug
            socket->connect(socket, &QTcpSocket::connected, [] {
                qDebug() << "Socket connected";
            });

            socket->connect(socket, &QTcpSocket::disconnected, [] {
                qDebug() << "Socket disconnected";
            });
            socket->connect(socket, &QTcpSocket::errorOccurred,
                [](QAbstractSocket::SocketError error) {
                    qDebug() << "Socket error:" << error;
            });
        }
        m_transport->disconnect(m_transport,nullptr,nullptr,nullptr);
        return m_transport;
    }
    void disconnect() {
        if (m_transport != nullptr) {
            m_transport->deleteLater();
        }
        m_transport = nullptr;
    }
    ~NetworkService() {
        disconnect();
    }

};

#endif //CATAN_NETWORKSERVICE_HPP