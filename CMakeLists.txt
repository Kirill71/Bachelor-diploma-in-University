cmake_minimum_required(VERSION 3.18)
project(Diploma VERSION 1.0.1 LANGUAGES CXX)

set(CUSTOM_INCLUDE_DIR "Headers")

include_directories(${CUSTOM_INCLUDE_DIR})

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCUSTOM_CMAKE_PREFIX_PATH)

if (NOT DEFINED CUSTOM_CMAKE_PREFIX_PATH OR CUSTOM_CMAKE_PREFIX_PATH STREQUAL "")
    message(FATAL_ERROR "You have to specify your own Qt prefix path using -DCUSTOM_CMAKE_PREFIX_PATH argument.")
else()
    set(CMAKE_PREFIX_PATH ${CUSTOM_CMAKE_PREFIX_PATH})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Core
    Gui
    Charts
)

set(HEADERS
    Headers/Model/Gauss.hpp
    Headers/Model/Matrix.hpp
    Headers/Model/PhysicalModel.hpp
    Headers/Utils/Log.hpp
    Headers/Utils/Utils.hpp
    Headers/Controllers/ChartController.hpp
    Headers/Views/MainWindow.hpp
    Headers/Views/ChartBuilder.hpp
    Headers/Views/CharBuilderDirector.hpp
    Headers/Views/NumbersDelegate.hpp
)

set(SOURCES
    Sources/Model/Gauss.cpp
    Sources/Model/Matrix.cpp
    Sources/Model/PhysicalModel.cpp
    Sources/Utils/Log.cpp
    Sources/Utils/Utils.cpp
    Sources/ph/ph.cpp
    Sources/Controllers/ChartController.cpp
    Sources/Views/mainwindow.ui
    Sources/Views/MainWindow.cpp
    Sources/Views/ChartBuilder.cpp
    Sources/Views/ChartBuilderDirector.cpp
    Sources/Views/NumbersDelegate.cpp
)

add_executable(unit-tests
    Headers/Tests/testslib.hpp
    Sources/Tests/testMain.cpp
    Sources/Model/Matrix.cpp
    Headers/Model/Matrix.hpp
)

add_executable(ips
    ${HEADERS}
    ${SOURCES}
     main.cpp
)

target_precompile_headers(ips PUBLIC ${CUSTOM_INCLUDE_DIR}/ph/ph.hpp)

set(COMMON_LIBS
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Charts
)

if (UNIX)
    target_link_libraries(ips
        ${COMMON_LIBS}
        pthread
    )

    target_link_libraries(unit-tests
        ${COMMON_LIBS}
        pthread
    )
else()
    target_link_libraries(ips
        ${COMMON_LIBS}
    )
    target_link_libraries(unit-tests
        ${COMMON_LIBS}
    )
endif()
