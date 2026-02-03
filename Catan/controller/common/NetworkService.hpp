//
// Created by matija on 2/2/26.
//

#ifndef CATAN_NETWORKSERVICE_HPP
#define CATAN_NETWORKSERVICE_HPP
#include <NetworkTransport.h>
#include <QSettings>

class NetworkService {
private:
    NetworkTransport* m_transport;

public:
    NetworkService() = default;
    NetworkTransport* getTransport() {
        if (m_transport == nullptr) {
            QSettings settings("./resources/client.ini", QSettings::IniFormat);

            QString host = settings.value("network/host", "127.0.0.1").toString();
            quint16 port = settings.value("network/port", 3460).toUInt();

            QTcpSocket* socket = new QTcpSocket();
            socket->connectToHost(host, port);
            m_transport = new NetworkTransport(socket);
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

};

#endif //CATAN_NETWORKSERVICE_HPP