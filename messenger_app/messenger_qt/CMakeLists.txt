cmake_minimum_required(VERSION 3.16)
project(messenger_qt)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(messenger_qt
    main.cpp
    database.cpp
    sqlite3.c
)

target_link_libraries(messenger_qt Qt6::Widgets)
