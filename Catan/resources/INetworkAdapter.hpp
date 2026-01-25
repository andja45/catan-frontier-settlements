//
// Created by matija on 1/24/26.
//

#ifndef CATAN_INETWORKADAPTER_HPP
#define CATAN_INETWORKADAPTER_HPP
#include <memory>
#include <qtmetamacros.h>

namespace net {
    class Envelope;
}

class NetworkTransport;

class INetworkAdapter {
private:
    std::unique_ptr<NetworkTransport> m_transport;
    uint32_t m_nextSeq = 1;
public:
    virtual ~INetworkAdapter() = default;
    void connectToServer(std::string ipAddress, uint16_t port);
    virtual void handleEnvelope(const net::Envelope&) = 0;
    std::unique_ptr<NetworkTransport> transferTransport();
    void transferTransport(std::unique_ptr<NetworkTransport> transport);

    void receiveEnvelope(net::Envelope& env);

};

#endif //CATAN_INETWORKADAPTER_HPP