cmake_minimum_required(VERSION 3.15)
project(PolyGameX VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_EXAMPLES "Build example projects" ON)

# Find packages
find_package(OpenGL REQUIRED)

# GLFW (for window management)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLAD (OpenGL loader)
add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC external/glad/include)

# GLM (math library)
add_subdirectory(external/glm EXCLUDE_FROM_ALL)

# stb_image (image loading)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE external/stb)

# Core engine library
add_library(PolyGameXCore
    src/core/Engine.cpp
    src/core/Logger.cpp
    src/rendering/Renderer.cpp
    src/rendering/Shader.cpp
    src/rendering/Camera.cpp
    src/rendering/Mesh.cpp
    src/rendering/Texture.cpp
    src/physics/PhysicsEngine.cpp
    src/physics/Collider.cpp
    src/physics/RigidBody.cpp
    src/ecs/ECS.cpp
    src/ecs/Entity.cpp
    src/ecs/Component.cpp
    src/scene/Scene.cpp
    src/scene/SceneManager.cpp
    src/math/Transform.cpp
    src/utils/FileSystem.cpp
)

target_include_directories(PolyGameXCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(PolyGameXCore PUBLIC
    ${OPENGL_LIBRARIES}
    glad
    glm
    stb_image
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(PolyGameXCore PUBLIC opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(PolyGameXCore PUBLIC GL X11 pthread dl)
elseif(APPLE)
    target_link_libraries(PolyGameXCore PUBLIC "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()

# Export symbols for DLL on Windows
if(WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(PolyGameXCore PRIVATE POLYGAMEX_EXPORTS)
    target_compile_definitions(PolyGameXCore INTERFACE POLYGAMEX_IMPORTS)
endif()

# Example application
if(BUILD_EXAMPLES)
    add_executable(PolyGameXDemo
        examples/demo/main.cpp
    )
    target_link_libraries(PolyGameXDemo PRIVATE PolyGameXCore)
endif()

# Installation
install(TARGETS PolyGameXCore
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Print configuration
message(STATUS "PolyGameX Configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Shared libs: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
