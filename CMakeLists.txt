cmake_minimum_required(VERSION 3.16)

project(girlpp VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_standard_project_setup()

qt_add_executable(girlpp
    src/main.cpp
    src/jid.cpp
    src/stanza_stream.cpp
)

target_link_libraries(girlpp PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network
)
