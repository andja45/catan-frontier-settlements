//
// Created by matija on 1/24/26.
//

#include "INetworkAdapter.hpp"

#include <envelope.pb.h>

void INetworkAdapter::receiveEnvelope(net::Envelope &env) {
    if (env.seq())
        throw std::runtime_error("Out of sync"); //TODO react to error
    handleEnvelope(env);
}

std::unique_ptr<NetworkTransport> INetworkAdapter::transferTransport() {
    return std::move(m_transport);
}

void INetworkAdapter::transferTransport(std::unique_ptr<NetworkTransport> transport) {
    transport = std::move(transport);

}
