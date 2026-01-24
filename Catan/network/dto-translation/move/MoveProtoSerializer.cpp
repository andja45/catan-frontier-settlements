//
// Created by jovan on 20/01/2026.
//


#include <cstdint>
#include "MoveProtoSerializer.h"

#include "board/Edge.h"

#include "move/turn/RollDiceMove.h"
#include "move/turn/EndTurnMove.h"
#include "move/build/BuildRoadMove.h"
#include "move/build/BuildSettlementMove.h"
#include "move/build/BuildCityMove.h"
#include "move/robber/DiscardCardsMove.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"
#include "move/trade/BankTradeMove.h"
#include "move/trade/PlayerTradeAcceptMove.h"
#include "move/trade/PlayerTradeRequestMove.h"
#include "move/trade/PlayerTradeResponseMove.h"
#include "move/devcard/BuyDevCardMove.h"
#include "move/devcard/MonopolyMove.h"
#include "move/devcard/PlayDevCardMove.h"
#include "move/devcard/YearOfPlentyMove.h"

#include "ProtoHeaders.hpp"

net::Move MoveProtoSerializer::toProto(const Move& move) {
    net::Move proto;


    proto.set_player_id(move.getPlayerId());

    switch (move.type()) {

        case MoveType::RollDice: {
            proto.set_type(net::Move::RollDice);
            proto.mutable_roll_dice();
            break;
        }

        case MoveType::EndTurn: {
            proto.set_type(net::Move::EndTurn);
            proto.mutable_end_turn();
            break;
        }

        case MoveType::BuildRoad: {
            proto.set_type(net::Move::BuildRoad);
            auto* m = static_cast<const BuildRoadMove*>(&move);
            proto.mutable_build_road()->set_edge_id(m->getEdgeId());
            break;
        }

        case MoveType::BuildSettlement: {
            proto.set_type(net::Move::BuildSettlement);
            auto* m = static_cast<const BuildSettlementMove*>(&move);
            proto.mutable_build_settlement()->set_node_id(m->getNodeId());
            break;
        }

        case MoveType::BuildCity: {
            proto.set_type(net::Move::BuildCity);
            auto* m = static_cast<const BuildCityMove*>(&move);
            proto.mutable_build_city()->set_node_id(m->getNodeId());
            break;
        }



        case MoveType::DiscardCards: {
            proto.set_type(net::Move::DiscardCards);
            auto* m = static_cast<const DiscardCardsMove*>(&move);

            auto* proto_map = proto.mutable_discard_cards()->mutable_discarded();
            for (auto const& [res, count] : m->getDiscarded()) {
                (*proto_map)[static_cast<int32_t>(res)] = count;
            }
            break;
        }

        case MoveType::SetRobber: {
            proto.set_type(net::Move::SetRobber);
            auto* m = static_cast<const SetRobberMove*>(&move);
            proto.mutable_set_robber()->set_tile_id(m->getTileId());
            break;
        }

        case MoveType::StealCard: {
            proto.set_type(net::Move::StealCard);
            auto* m = static_cast<const StealCardMove*>(&move);
            proto.mutable_steal_card()->set_victim_player_id(m->getVictimPlayerId());
            break;
        }

        case MoveType::BuyDevCard: {
            proto.set_type(net::Move::BuyDevCard);
            proto.mutable_buy_dev_card();
            break;
        }

        case MoveType::Monopoly: {
            proto.set_type(net::Move::Monopoly);
            auto* m = static_cast<const MonopolyMove*>(&move);
            proto.mutable_monopoly()->set_resource_type(static_cast<net::ResourceType>(m->getResource()));
            break;
        }

        case MoveType::PlayDevCard: {
            proto.set_type(net::Move::PlayDevCard);
            auto* m = static_cast<const PlayDevCardMove*>(&move);
            proto.mutable_play_dev_card()->set_card_type(static_cast<net::DevCardType>(static_cast<int32_t>(m->getCardType())));
            break;
        }

        case MoveType::YearOfPlenty: {
            proto.set_type(net::Move::YearOfPlenty);
            auto* m = static_cast<const YearOfPlentyMove*>(&move);
            auto* yop_proto = proto.mutable_year_of_plenty();
            yop_proto->set_resource_1(static_cast<net::ResourceType>(m->getFirst()));
            yop_proto->set_resource_2(static_cast<net::ResourceType>(m->getSecond()));
            break;
        }

        case MoveType::BankTrade: {
            proto.set_type(net::Move::BankTrade);
            auto* m = static_cast<const BankTradeMove*>(&move);
            auto* bank_proto = proto.mutable_bank_trade();
            bank_proto->set_get_resource(static_cast<net::ResourceType>(m->getGive()));
            bank_proto->set_get_resource(static_cast<net::ResourceType>(m->getReceive()));
            break;
        }

        case MoveType::PlayerTradeRequest: {
            proto.set_type(net::Move::PlayerTradeRequest);
            auto* m = static_cast<const PlayerTradeRequestMove*>(&move);
            auto* req_proto = proto.mutable_trade_request();
            for (auto const& [res, count] : m->getGive()) {
                (*req_proto->mutable_give_resource())[static_cast<int32_t>(res)] = count;
            }
            for (auto const& [res, count] : m->getReceive()) {
                (*req_proto->mutable_get_resource())[static_cast<int32_t>(res)] = count;
            }
            break;
        }

        case MoveType::PlayerTradeResponse: {
            proto.set_type(net::Move::PlayerTradeResponse);
            auto* m = static_cast<const PlayerTradeResponseMove*>(&move);
            proto.mutable_trade_response()->set_trade_request_id(m->getTradeRequestId());
            break;
        }

        case MoveType::PlayerTradeAccept: {
            proto.set_type(net::Move::PlayerTradeAccept);
            auto* m = static_cast<const PlayerTradeAcceptMove*>(&move);
            auto* acc_proto = proto.mutable_trade_accept();
            acc_proto->set_accept_player_id(m->getAcceptedPlayerId());
            acc_proto->set_trade_id(m->getTradeId());
            break;
        }

        default:

            break;
    }

    return proto;
}