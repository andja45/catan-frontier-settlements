//
// Created by jovan on 19/01/2026.
//

#include <memory>
#include  "MoveProtoFactory.h"
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
#include "move.pb.h"


std::unique_ptr<Move> MoveProtoFactory::fromProto(const catan::Move& proto) {

    PlayerId pId = proto.player_id();

    switch (proto.payload_case()) {

      case catan::Move::kRollDice:
            return std::make_unique<RollDiceMove>(pId);

        case catan::Move::kEndTurn:
            return std::make_unique<EndTurnMove>(pId);

        case catan::Move::kBuildRoad:
            return std::make_unique<BuildRoadMove>(pId, proto.build_road().edge_id());

        case catan::Move::kBuildSettlement:
            return std::make_unique<BuildSettlementMove>(pId, proto.build_settlement().node_id());

        case catan::Move::kBuildCity:
            return std::make_unique<BuildCityMove>(pId, proto.build_city().node_id());

        case catan::Move::kDiscardCards: {
            ResourcePack discarded;
            for (auto const& [res_int, count] : proto.discard_cards().discarded()) {
                discarded[static_cast<ResourceType>(res_int)] = count;
            }
            return std::make_unique<DiscardCardsMove>(pId, discarded);
        }

        case catan::Move::kSetRobber:
            return std::make_unique<SetRobberMove>(pId, proto.set_robber().tile_id());

        case catan::Move::kStealCard:
            return std::make_unique<StealCardMove>(pId, proto.steal_card().victim_player_id());

        case catan::Move::kBuyDevCard:
            return std::make_unique<BuyDevCardMove>(pId);

        case catan::Move::kMonopoly:
            return std::make_unique<MonopolyMove>(pId, static_cast<ResourceType>(proto.monopoly().resource_type()));

        case catan::Move::kPlayDevCard:
            return std::make_unique<PlayDevCardMove>(pId, static_cast<DevCardType>(proto.play_dev_card().card_type()));

        case catan::Move::kYearOfPlenty:
            return std::make_unique<YearOfPlentyMove>(pId,
                static_cast<ResourceType>(proto.year_of_plenty().resource_1()),
                static_cast<ResourceType>(proto.year_of_plenty().resource_2()));

        case catan::Move::kBankTrade:
            return std::make_unique<BankTradeMove>(pId,
                static_cast<ResourceType>(proto.bank_trade().give_resource()),
                static_cast<ResourceType>(proto.bank_trade().get_resource()));

        case catan::Move::kTradeRequest: {
            ResourcePack give, receive;
            for (auto const& [res, count] : proto.trade_request().give_resource())
                give[static_cast<ResourceType>(res)] = count;
            for (auto const& [res, count] : proto.trade_request().get_resource())
                receive[static_cast<ResourceType>(res)] = count;
            return std::make_unique<PlayerTradeRequestMove>(pId, give, receive);
        }

        case catan::Move::kTradeResponse:
            return std::make_unique<PlayerTradeResponseMove>(pId, proto.trade_response().trade_request_id());

        case catan::Move::kTradeAccept:
            return std::make_unique<PlayerTradeAcceptMove>(pId,
                proto.trade_accept().accept_player_id(),
                proto.trade_accept().trade_id());
        default:
            return nullptr;
    }
}