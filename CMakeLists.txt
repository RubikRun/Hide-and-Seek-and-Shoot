cmake_minimum_required (VERSION 3.18.4)

project(hide-and-seek-and-shoot)

# Configure compilation
add_executable(game main.cpp Game/Game.cpp)

# Configure SFML
target_include_directories(game
    PUBLIC SFML-2.5.1/include/)
target_link_directories(game
    PUBLIC SFML-2.5.1/lib/
)
target_link_libraries(game
    sfml-graphics
    sfml-window
    sfml-system
)