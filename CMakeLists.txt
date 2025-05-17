cmake_minimum_required(VERSION 3.16)

project(BounceLab VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)
endif()

# Find Box2D package
find_package(box2d QUIET)

# If Box2D is not found, use FetchContent to download and build it
if(NOT box2d_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.2
    )
    # Disable Box2D tests and examples
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(box2d)
endif()

# BounceLab source files
set(BOUNCELAB_SRCS
    main.cpp
    mainwindow.cpp
    simulationview.cpp
)

set(BOUNCELAB_HDRS
    mainwindow.h
    simulationview.h
)

set(BOUNCELAB_UIS
    mainwindow.ui
)

# Resource files
set(RESOURCE_FILES
    Images.qrc
)

# Create BounceLab executable
add_executable(${PROJECT_NAME}
    ${BOUNCELAB_SRCS}
    ${BOUNCELAB_HDRS}
    ${BOUNCELAB_UIS}
    ${RESOURCE_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    box2d
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::OpenGLWidgets
)

# Include directories for main application
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Install rules
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
    install(TARGETS ${PROJECT_NAME} DESTINATION ".")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/images" DESTINATION ".")
elseif(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/images" DESTINATION share/${PROJECT_NAME})
endif()
