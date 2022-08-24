project(QVIApp)
cmake_minimum_required(VERSION 3.1)

## Target
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

set(QVIApp_SRCS AsyncSerial.cpp QAsyncSerial.cpp QVoltageIndicatorApp.cpp main.cpp)
set(QVIApp_HEADERS AsyncSerial.hpp QAsyncSerial.hpp QVoltageIndicatorApp.hpp)
list(TRANSFORM QVIApp_SRCS PREPEND "src/")
list(TRANSFORM QVIApp_HEADERS PREPEND "src/")
add_executable(QVoltageIndicator ${QVIApp_SRCS})

## For Qt Integration
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(QVoltageIndicator Qt5::Widgets)

## Link libraries
set(BOOST_LIBS date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(QVoltageIndicator ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(QVoltageIndicator ${CMAKE_THREAD_LIBS_INIT})
