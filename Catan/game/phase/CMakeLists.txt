add_library(catan_phase_lib INTERFACE
        TurnPhase.h
)

target_include_directories(catan_phase_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)