cmake_minimum_required(VERSION 3.2)
project(dummy_sph)


set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find the QtWidgets library
set(CMAKE_PREFIX_PATH "/home/nikita/Qt5.7.0/5.7/gcc_64")

find_package(Qt5Widgets)

set(
        SOURCE_FILES
        main.cpp

        Particile.h Particile.cpp

        Dummy_flow.h Dummy_flow.cpp

        Dummy_flow_drawer.cpp Dummy_flow_drawer.h

        MainWindow.cpp MainWindow.h

        myqgraphicsview.h MyQGraphicsView.cpp
        dW_functions.h)

add_executable(dummy_sph ${SOURCE_FILES})

target_link_libraries(dummy_sph Qt5::Widgets)

##############TESTING############
