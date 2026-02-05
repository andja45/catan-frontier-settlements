//
// Created by matija on 1/24/26.
//

#ifndef CATAN_LOBBYNETWORKADAPTER_HPP
#define CATAN_LOBBYNETWORKADAPTER_HPP
#include <memory>
#include <NetworkTransport.h>
#include <board/Board.h>
#include <model/GameConfig.h>

class LobbyNetworkAdapter : public QObject {
    Q_OBJECT
public:
    explicit LobbyNetworkAdapter(QObject* parent = nullptr);
    void setTransport(NetworkTransport* transport);

    void sendLeave() const;
    void sendConfig(const GameConfig& conf) const;
    void sendStartRequest(const Board& board) const;
    void sendJoined();

    void onError(const std::string& error);
    void onDisconnected();
    signals:
        void configReceived(const GameConfig& config);
        void gameStarted(PlayerId myId, int32_t gameSeed, const GameConfig& config, Board* board);
        void disconnected();
        void errored(const std::string&);
private:
    NetworkTransport* m_transport;

    net::Envelope wrapConfig(const GameConfig& config) const;
    net::Envelope wrapStartRequest(const Board& board) const;
    net::Envelope wrapLeave() const;

    net::Envelope prepareEnvelope() const;

    void onEnvelope(const net::Envelope& env);

    void handleConfig(const net::Envelope& env);
    void handleGameStarted(const net::Envelope& env);

};


#endif //CATAN_LOBBYNETWORKADAPTER_HPP