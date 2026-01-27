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

    void sendLeave() const;
    void sendConfig(const GameConfig& conf) const;
    void sendStartRequest(const Board& board) const;

    std::unique_ptr<NetworkTransport> getTransport();
    void setTransport(std::unique_ptr<NetworkTransport> transport);

    signals:
        void configReceived(const GameConfig& config);
        void gameStarted(PlayerId myId, int32_t gameSeed, const GameConfig& config, std::unique_ptr<Board> board);

private:
    std::unique_ptr<NetworkTransport> m_transport;

    net::Envelope wrapConfig(const GameConfig& config) const;
    net::Envelope wrapStartRequest(const Board& board) const;
    net::Envelope wrapLeave() const;

    net::Envelope prepareEnvelope() const;

    void onEnvelope(const net::Envelope& env);

    void handleConfig(const net::Envelope& env);
    void handleGameStarted(const net::Envelope& env);

};


#endif //CATAN_LOBBYNETWORKADAPTER_HPP