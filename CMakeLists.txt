cmake_minimum_required(VERSION 3.1)
project(mariobreaksout)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/piksel)

set(SOURCES
    src/main.cpp
    src/app.cpp
    src/ball.cpp
    src/paddle.cpp
    src/brick.cpp
    src/aabb.cpp
    src/character.cpp
    src/enemy.cpp
    src/portal.cpp
    src/firework.cpp
    src/particle.cpp
)
add_executable(mariobreaksout ${SOURCES})
target_include_directories(mariobreaksout
    PUBLIC src
    PUBLIC lib/piksel/src
)
target_link_libraries(mariobreaksout
    piksel
)