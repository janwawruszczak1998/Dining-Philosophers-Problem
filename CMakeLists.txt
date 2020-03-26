cmake_minimum_required(VERSION 3.15)
project(Dining_Philosophers)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)



add_executable(Dining_Philosophers main.cpp Philosopher.h Fork.h)

target_link_directories(Dining_Philosophers PUBLIC
        "$(PROJECT_BINARY_DIR)"
        )
target_link_libraries(Dining_Philosophers PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        ${CURSES_LIBRARIES}
        )