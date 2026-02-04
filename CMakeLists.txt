cmake_minimum_required(VERSION 3.20)
project(2026_chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(chess_core
  src/Board.cpp
  src/GameState.cpp
  src/MoveRules.cpp
)

target_include_directories(chess_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

option(ENABLE_TESTING "Enable unit tests" ON)
if(ENABLE_TESTING)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(chess_tests
    tests/test_board_setup.cpp
    tests/test_game_state.cpp
    tests/test_move_rules.cpp
  )
  target_link_libraries(chess_tests PRIVATE chess_core gtest_main)

  include(GoogleTest)
  gtest_discover_tests(chess_tests)
endif()
