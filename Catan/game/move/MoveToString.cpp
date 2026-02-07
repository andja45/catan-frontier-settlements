#include "MoveToString.h"

#include <sstream>

#include "move/Move.h"
#include "model/GameSession.h"

#include "types/ResourceType.h"
#include "types/DevCardType.h"

#include "move/turn/RollDiceMove.h"
#include "move/turn/EndTurnMove.h"
#include "move/turn/PlayerLeaveMove.hpp"

#include "move/build/BuildRoadMove.h"
#include "move/build/BuildSettlementMove.h"
#include "move/build/BuildCityMove.h"

#include "move/robber/DiscardCardsMove.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"

#include "move/trade/BankTradeMove.h"
#include "move/trade/PlayerTradeRequestMove.h"
#include "move/trade/PlayerTradeResponseMove.h"
#include "move/trade/PlayerTradeAcceptMove.h"

#include "move/devcard/BuyDevCardMove.h"
#include "move/devcard/PlayDevCardMove.h"
#include "move/devcard/YearOfPlentyMove.h"
#include "move/devcard/MonopolyMove.h"

namespace {
std::string playerName(const GameSession* session, PlayerId id) {
    return session ? session->player(id).getName() : std::to_string(id);
}

std::string packToString(const ResourcePack& pack) {
    std::ostringstream oss;
    bool first = true;
    for (const auto& [res, cnt] : pack) {
        if (cnt <= 0) continue;
        if (!first) oss << ", ";
        first = false;
        oss << cnt << " " << toString(res);
    }
    return first ? std::string("nothing") : oss.str();
}
}

std::string moveToString(const Move& move, const GameSession* session) {
    std::ostringstream oss;
    const auto actor = playerName(session, move.getPlayerId());

    switch (move.type()) {
        case MoveType::BuildRoad: {
            auto const& m = static_cast<const BuildRoadMove&>(move);
            oss << actor << " placed a road on edge #" << m.getEdgeId();
            break;
        }
        case MoveType::BuildSettlement: {
            auto const& m = static_cast<const BuildSettlementMove&>(move);
            oss << actor << " placed a settlement on node #" << m.getNodeId();
            break;
        }
        case MoveType::BuildCity: {
            auto const& m = static_cast<const BuildCityMove&>(move);
            oss << actor << " built a city on node #" << m.getNodeId();
            break;
        }
        case MoveType::RollDice: {
            auto const& m = static_cast<const RollDiceMove&>(move);
            auto [d1, d2] = m.getDiceRoll();
            oss << actor << " rolled " << (d1 + d2);
            break;
        }
        case MoveType::EndTurn:
            oss << actor << " ended turn";
            break;

        case MoveType::DiscardCards: {
            auto const& m = static_cast<const DiscardCardsMove&>(move);
            oss << actor << " discarded " << packToString(m.getDiscarded());
            break;
        }
        case MoveType::SetRobber: {
            auto const& m = static_cast<const SetRobberMove&>(move);
            oss << actor << " moved robber to tile #" << m.getTileId();
            break;
        }
        case MoveType::StealCard: {
            auto const& m = static_cast<const StealCardMove&>(move);
            oss << actor << " stole a card from " << playerName(session, m.getVictimPlayerId());
            break;
        }

        case MoveType::BankTrade: {
            auto const& m = static_cast<const BankTradeMove&>(move);
            oss << actor << " traded with bank: " << toString(m.getGive())
                << " -> " << toString(m.getReceive());
            break;
        }
        case MoveType::PlayerTradeRequest: {
            auto const& m = static_cast<const PlayerTradeRequestMove&>(move);
            oss << actor << " sent a trade: give " << packToString(m.getGive())
                << " / receive " << packToString(m.getReceive());
            break;
        }
        case MoveType::PlayerTradeResponse: {
            auto const& m = static_cast<const PlayerTradeResponseMove&>(move);
            oss << actor << (m.getIsPositive() ? " accepted" : " declined")
                << " trade #" << m.getTradeRequestId();
            break;
        }
        case MoveType::PlayerTradeAccept: {
            auto const& m = static_cast<const PlayerTradeAcceptMove&>(move);
            oss << actor << " completed trade #" << m.getTradeId()
                << " with " << playerName(session, m.getAcceptedPlayerId());
            break;
        }

        case MoveType::BuyDevCard: {
            auto const& m = static_cast<const BuyDevCardMove&>(move);
            (void)m;
            oss << actor << " bought a dev card";
            break;
        }
        case MoveType::PlayDevCard: {
            auto const& m = static_cast<const PlayDevCardMove&>(move);
            oss << actor << " used " << toString(m.getCardType());
            break;
        }
        case MoveType::YearOfPlenty: {
            auto const& m = static_cast<const YearOfPlentyMove&>(move);
            oss << actor << " used Year of Plenty: " << toString(m.getFirst())
                << ", " << toString(m.getSecond());
            break;
        }
        case MoveType::Monopoly: {
            auto const& m = static_cast<const MonopolyMove&>(move);
            oss << actor << " used Monopoly: " << toString(m.getResource());
            break;
        }

        case MoveType::PlayerLeave:
            oss << actor << " left the game";
            break;

        default:
            oss << actor << " made a move";
            break;
    }

    return oss.str();
}
