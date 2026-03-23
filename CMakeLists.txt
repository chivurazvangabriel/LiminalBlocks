cmake_minimum_required(VERSION 3.26)

# Project name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include template modules
include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################
# Executable

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        glad.c
        LiminalBlocks/Core/Public/Camera.h
        LiminalBlocks/Core/Private/Camera.cpp
        LiminalBlocks/Core/Public/Object.h
        LiminalBlocks/Core/Private/Object.cpp
        LiminalBlocks/Core/Interfaces/Private/ITickable.cpp
        LiminalBlocks/Core/Interfaces/Public/ITickable.h
        LiminalBlocks/Core/Interfaces/Private/BaseInterface.cpp
        LiminalBlocks/Core/Interfaces/Public/BaseInterface.h
        LiminalBlocks/Core/Private/GameInstance.cpp
        LiminalBlocks/Core/Public/GameInstance.h
        LiminalBlocks/Core/Private/InputManager.cpp
        LiminalBlocks/Core/Public/InputManager.h
        LiminalBlocks/Core/Private/TickManager.cpp
        LiminalBlocks/Core/Public/TickManager.h
        LiminalBlocks/Core/Private/Block.cpp
        LiminalBlocks/Core/Public/Block.h
        LiminalBlocks/Core/Statics/ColorStatics.h
        LiminalBlocks/Core/Private/BlockRenderer.cpp
        LiminalBlocks/Core/Public/BlockRenderer.h
        LiminalBlocks/Core/Private/World.cpp
        LiminalBlocks/Core/Public/World.h
        data/Blocks.h
        LiminalBlocks/Core/Private/MulticastDelegate.cpp
        LiminalBlocks/Core/Public/MulticastDelegate.h
        LiminalBlocks/Core/Private/Player.cpp
        LiminalBlocks/Core/Public/Player.h
        LiminalBlocks/Core/Private/PostProcessingManager.cpp
        LiminalBlocks/Core/Public/PostProcessingManager.h
        LiminalBlocks/Core/Statics/ShaderStatics.h
        LiminalBlocks/Core/Interfaces/Private/IInputHandler.cpp
        LiminalBlocks/Core/Interfaces/Public/IInputHandler.h
        LiminalBlocks/Core/Private/BaseSubsystem.cpp
        LiminalBlocks/Core/Public/BaseSubsystem.h
        LiminalBlocks/Core/Private/SubsystemManager.cpp
        LiminalBlocks/Core/Public/SubsystemManager.h
        LiminalBlocks/Core/Private/WindowManager.cpp
        LiminalBlocks/Core/Public/WindowManager.h
        LiminalBlocks/Core/Private/BaseObjectComponent.cpp
        LiminalBlocks/Core/Public/BaseObjectComponent.h
        LiminalBlocks/Core/Private/ColliderComponent.cpp
        LiminalBlocks/Core/Public/ColliderComponent.h
        LiminalBlocks/Core/Private/Game.cpp
        LiminalBlocks/Core/Public/Game.h
        LiminalBlocks/Core/Private/WorldGenerator.cpp
        LiminalBlocks/Core/Public/WorldGenerator.h
)

###############################################################################
# SFML configuration (local library inside libs/)

###############################################################################
# SFML configuration

# 1. Tell CMake where to look for SFML's configuration files
set(SFML_DIR "${PROJECT_SOURCE_DIR}/libs/SFML-3.0.2/lib/cmake/SFML")

# 2. Find the package properly
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)
# 3. Link using the SFML targets (this handles includes and defines automatically)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        opengl32
        glu32
)

target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE "LiminalBlocks")
target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE "libs")

###############################################################################
# Compiler flags

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################
# Installation

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})

if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

###############################################################################
# Copy runtime files

copy_files(
        FILES tastatura.txt
        COPY_TO_DESTINATION
        TARGET_NAME ${MAIN_EXECUTABLE_NAME}
)
