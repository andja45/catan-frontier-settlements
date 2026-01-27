//
// Created by matija on 1/25/26.
//

#ifndef CATAN_HOSTJOINNETWORKADAPTER_HPP
#define CATAN_HOSTJOINNETWORKADAPTER_HPP
#include <cstdint>
#include <memory>
#include <NetworkTransport.h>
#include <string>

class HostJoinNetworkAdapter : public QObject {
    Q_OBJECT
private:
    std::unique_ptr<NetworkTransport> m_transport;
    net::Envelope wrapHost(std::string gameName, std::string hostName);
    net::Envelope wrapJoin(std::string gameName, std::string playerName);
public:
    explicit HostJoinNetworkAdapter(QObject* parent = nullptr){}

    void connectTo(const std::string& addr, std::uint16_t port);
    std::unique_ptr<NetworkTransport> getTransport();

    void sendHost(std::string gameName, std::string hostName);
    void sendJoin(std::string gameName, std::string playerName);

    void onEnvelope(const net::Envelope& env);
    void handleAcceptResponse(const net::Envelope& env);
    void handleRejectResponse(const net::Envelope& env);

signals:
    void rejectReceived();
    void acceptReceived();


};

#endif //CATAN_HOSTJOINNETWORKADAPTER_HPP