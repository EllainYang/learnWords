cmake_minimum_required(VERSION 3.0.2)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")


# headers also required to add in add_executable, cause otherwise it wont find headers
add_executable(MyProject ${HEADERS} ${SOURCES})


target_link_libraries(MyProject Qt5::Widgets Qt5::Sql)