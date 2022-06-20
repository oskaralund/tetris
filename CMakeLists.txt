cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tetris LANGUAGES CXX C)

add_executable(main src/main.cpp
                    src/board.cpp
                    src/piece.cpp
                    src/game.cpp)
target_include_directories(main PUBLIC include)
target_link_libraries(main PUBLIC -lSDL2 -lSDL_ttf)
target_compile_features(main PUBLIC cxx_std_20)
