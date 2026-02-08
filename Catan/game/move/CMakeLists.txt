target_sources(catan_game PRIVATE
        Move.cpp
        Move.h
        MoveType.h
        BoardMove.h
        # build
        build/BuildRoadMove.cpp
        build/BuildRoadMove.h
        build/BuildSettlementMove.cpp
        build/BuildSettlementMove.h
        build/BuildCityMove.cpp
        build/BuildCityMove.h
        # turn
        turn/RollDiceMove.cpp
        turn/RollDiceMove.h
        turn/EndTurnMove.cpp
        turn/EndTurnMove.h
        turn/PlayerLeaveMove.cpp
        turn/PlayerLeaveMove.hpp
        # trade
        trade/BankTradeMove.cpp
        trade/BankTradeMove.h
        trade/PlayerTradeRequestMove.cpp
        trade/PlayerTradeRequestMove.h
        trade/PlayerTradeResponseMove.cpp
        trade/PlayerTradeResponseMove.h
        trade/PlayerTradeAcceptMove.cpp
        trade/PlayerTradeAcceptMove.h
        # robber
        robber/SetRobberMove.cpp
        robber/SetRobberMove.h
        robber/DiscardCardsMove.cpp
        robber/DiscardCardsMove.h
        robber/StealCardMove.cpp
        robber/StealCardMove.h
        # devcard
        devcard/BuyDevCardMove.cpp
        devcard/BuyDevCardMove.h
        devcard/YearOfPlentyMove.cpp
        devcard/YearOfPlentyMove.h
        devcard/MonopolyMove.cpp
        devcard/MonopolyMove.h
        devcard/PlayDevCardMove.cpp
        devcard/PlayDevCardMove.h

        MoveToString.cpp
        MoveToString.h
)

target_include_directories(catan_game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/build
        ${CMAKE_CURRENT_SOURCE_DIR}/turn
        ${CMAKE_CURRENT_SOURCE_DIR}/trade
        ${CMAKE_CURRENT_SOURCE_DIR}/robber
        ${CMAKE_CURRENT_SOURCE_DIR}/devcard
)