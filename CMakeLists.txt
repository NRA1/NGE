cmake_minimum_required(VERSION 3.24)
project(NGE)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(lib/glad)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/assimp)
add_subdirectory(lib/glm)

#for profiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

#settings
add_compile_definitions(RESOURCES_PREFIX_PATH="../")

# BOOST
if (UNIX)
add_subdirectory(lib/boost/unix/core)
add_subdirectory(lib/boost/unix/static_assert)
add_subdirectory(lib/boost/unix/mp11)
add_subdirectory(lib/boost/unix/type_traits)
add_subdirectory(lib/boost/unix/predef)
add_subdirectory(lib/boost/unix/winapi)
add_subdirectory(lib/boost/unix/describe)
add_subdirectory(lib/boost/unix/container_hash)
add_subdirectory(lib/boost/unix/throw_exception)
add_subdirectory(lib/boost/unix/config)
add_subdirectory(lib/boost/unix/assert)
add_subdirectory(lib/boost/unix/array)
add_subdirectory(lib/boost/unix/stacktrace)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif ()

add_executable(NGE main.cpp infrastructure/logger.cpp infrastructure/logger.hpp infrastructure/assertions.hpp interface/window.cpp interface/window.hpp infrastructure/game.cpp infrastructure/game.hpp data_objects/size.cpp data_objects/size.hpp native/x_11_window.inl native/x_11_window.hpp native/abstract_native_window.hpp data_objects/rect.cpp data_objects/rect.hpp infrastructure/resource_loader.hpp infrastructure/resource_loader.cpp interface/abstract_stage.hpp world/object.cpp world/object.hpp world/abstractions/abstract_component.cpp world/abstractions/abstract_component.hpp enums/component_type.hpp data_objects/box.cpp data_objects/box.hpp world/mesh_component.cpp world/mesh_component.hpp data_objects/vec.hpp interface/world_stage.cpp interface/world_stage.hpp infrastructure/common_fallbacks.hpp data_objects/vertex.hpp data_objects/texture.cpp data_objects/texture.hpp data_objects/mesh.cpp data_objects/mesh.hpp data_objects/shader_program.cpp data_objects/shader_program.hpp infrastructure/type_helpers.hpp data_objects/model.cpp data_objects/model.hpp data_objects/image.cpp data_objects/image.hpp infrastructure/common_fallbacks.cpp enums/log_level.hpp world/camera.cpp world/camera.hpp data_objects/events/event.cpp data_objects/events/event.hpp enums/event_type.hpp data_objects/events/key_press_event.cpp data_objects/events/key_press_event.hpp enums/key.hpp native/native_helpers.hpp native/native_helpers.cpp enums/modifier.hpp enums/modifier.cpp world/abstractions/abstract_mesh_component.hpp world/object.inl world/ground.cpp world/ground.hpp data_objects/sprite.cpp data_objects/sprite.hpp world/abstractions/abstract_mesh_component.cpp)
target_link_libraries(NGE glad)
if (UNIX)
    target_link_libraries(NGE glx)
endif ()

if (WINDOWS)
    target_link_libraries(NGE wgl)
endif ()
target_link_libraries(NGE boost_stacktrace_basic)
target_link_libraries(NGE boost_filesystem)
target_link_libraries(NGE X11)
target_link_libraries(NGE stb_image)
target_link_libraries(NGE assimp)
target_link_libraries(NGE glm)

#set compiler warning level
target_compile_options(NGE PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wno-volatile>
        )