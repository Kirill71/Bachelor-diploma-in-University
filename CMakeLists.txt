
cmake_minimum_required(VERSION 3.18)
project(Diplom VERSION 1.0.1 LANGUAGES CXX)

include_directories(include/boost_1_65_1 Headers)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCUSTOMER_PREFIX_PATH)

if (NOT DEFINED CUSTOMER_PREFIX_PATH OR CUSTOMER_PREFIX_PATH STREQUAL "")
    message(FATAL_ERROR "You have to specify your own Qt prefix path using CMAKE_PREFIX_PATH argument.")
else()
    set(CMAKE_PREFIX_PATH ${CUSTOMER_PREFIX_PATH})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Widgets
        Core
        Gui
        Charts
        REQUIRED)

add_executable(Diplom
        Sources/Views/mainwindow.ui
    main.cpp
    Headers/Views/MainWindow.hpp # this header should be added here otherwise some magic from qt side is being happened.
    Sources/Views/MainWindow.cpp
    Sources/Model/Gauss.cpp
    Sources/Model/Matrix.cpp
        Sources/Model/PhysicalModel.cpp
    Sources/Tests/testMain.cpp
    Sources/ph/ph.cpp
    Sources/Utils/Log.cpp
    Sources/Views/ChartBuilder.cpp
    Sources/Views/ChartBuilderDirector.cpp
    Sources/Controllers/ChartController.cpp
    Sources/Utils/Utils.cpp
    Sources/Views/NumbersDelegate.cpp
)
target_precompile_headers(Diplom PUBLIC Headers/ph/ph.hpp)

if (UNIX)
    target_link_libraries(Diplom
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        pthread
        )
else()
    target_link_libraries(Diplom
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            Qt5::Charts
            )
endif()
