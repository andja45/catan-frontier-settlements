add_executable(Server
        main.cpp
)

target_link_libraries(Server
        PRIVATE
        catan_network_server
        catan_game
)

