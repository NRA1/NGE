cmake_minimum_required(VERSION 3.24)
project(NGE)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(lib/glad)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/assimp)
add_subdirectory(lib/glm)
add_subdirectory(lib/freetype)

#for profiler
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif()

#for WIN32
add_compile_definitions(WIN32_LEAN_AND_MEAN)
add_compile_definitions(NOMINMAX)

#settings
if(UNIX)
    add_compile_definitions(RESOURCES_PREFIX_PATH="../")
    add_compile_definitions(DIRECTORY_SEPARATOR='/')
elseif(WIN32)
    #add_compile_definitions(RESOURCES_PREFIX_PATH="..\\\\..\\\\..\\\\")
    add_compile_definitions(RESOURCES_PREFIX_PATH="..\\\\")
    add_compile_definitions(DIRECTORY_SEPARATOR='\\\\')
endif()

add_compile_definitions(DISPLAY_COLLIDERS=false)

add_executable(NGE WIN32 main.cpp infrastructure/logger.cpp infrastructure/logger.hpp infrastructure/assertions.hpp interface/game_window.cpp interface/game_window.hpp infrastructure/game.cpp infrastructure/game.hpp data_objects/size.cpp data_objects/size.hpp native/x_11_window.inl native/x_11_window.hpp native/abstract_native_window.hpp data_objects/rect.cpp data_objects/rect.hpp infrastructure/resource_loader.hpp infrastructure/resource_loader.cpp interface/abstract_stage.hpp world/object.cpp world/object.hpp world/abstractions/abstract_component.cpp world/abstractions/abstract_component.hpp enums/component_type.hpp data_objects/box.cpp data_objects/box.hpp world/mesh_component.cpp world/mesh_component.hpp data_objects/vec.hpp interface/world_stage.cpp interface/world_stage.hpp infrastructure/common_fallbacks.hpp data_objects/vertex.hpp data_objects/texture.cpp data_objects/texture.hpp data_objects/mesh.cpp data_objects/mesh.hpp data_objects/shader_program.cpp data_objects/shader_program.hpp infrastructure/type_helpers.hpp data_objects/model.cpp data_objects/model.hpp data_objects/image.cpp data_objects/image.hpp infrastructure/common_fallbacks.cpp enums/log_level.hpp world/camera.cpp world/camera.hpp data_objects/events/event.cpp data_objects/events/event.hpp enums/event_type.hpp data_objects/events/key_press_event.cpp data_objects/events/key_press_event.hpp enums/key.hpp native/native_helpers.hpp native/native_helpers.cpp enums/modifier.hpp enums/modifier.cpp world/abstractions/abstract_mesh_component.hpp world/object.inl world/ground.cpp world/ground.hpp data_objects/sprite.cpp data_objects/sprite.hpp world/abstractions/abstract_mesh_component.cpp data_objects/motion_vector.cpp data_objects/motion_vector.hpp data_objects/motion_manipulation_object.cpp data_objects/motion_manipulation_object.hpp data_objects/rotation.cpp data_objects/rotation.hpp world/input_component.cpp world/input_component.hpp world/abstractions/abstract_input_component.cpp world/abstractions/abstract_input_component.hpp data_objects/events/key_release_event.cpp data_objects/events/key_release_event.hpp data_objects/events/mouse_move_event.cpp data_objects/events/mouse_move_event.hpp native/abstract_native_window.inl interface/interface_helpers.hpp interface/interface_helpers.cpp data_objects/positioned_box.cpp data_objects/positioned_box.hpp data_objects/position.cpp data_objects/position.hpp world/debug_box.cpp world/debug_box.hpp native/win_32_window.hpp native/win_32_window.inl data_objects/winmain_config.hpp world/abstractions/abstract_npc_component.cpp world/abstractions/abstract_npc_component.hpp game/npc_component.cpp game/npc_component.hpp interface/graph_stage.cpp interface/graph_stage.hpp graph/widget.cpp graph/widget.hpp graph/rect_widget.cpp graph/rect_widget.hpp data_objects/character.cpp data_objects/character.hpp graph/text_widget.cpp graph/text_widget.hpp enums/shader_program_type.hpp data_objects/events/mouse_button_event.cpp data_objects/events/mouse_button_event.h enums/mouse_button.h data_objects/events/mouse_press_event.cpp data_objects/events/mouse_press_event.h data_objects/events/mouse_release_event.cpp data_objects/events/mouse_release_event.h world/abstractions/abstract_bar_component.cpp world/abstractions/abstract_bar_component.h world/bar_component.cpp world/bar_component.h world/property_component.h infrastructure/save_manager.cpp infrastructure/save_manager.hpp graph/button_widget.cpp graph/button_widget.hpp enums/layout_origin.hpp enums/layout_flags.hpp game/game_component_type.hpp interface/replay_stage.cpp interface/replay_stage.hpp data_objects/track_stamp.hpp main.hpp enums/key.cpp game/user_object_type.hpp)

# BOOST
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

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

    target_link_libraries(NGE boost_stacktrace_basic)
    target_link_libraries(NGE boost_filesystem)
elseif(WIN32)
    set(BOOST_ROOT C:/Boost)
    find_package(Boost 1.81 COMPONENTS stacktrace_noop filesystem REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    target_link_libraries(NGE ${Boost_LIBRARIES})
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif ()

target_link_libraries(NGE glad)
if (UNIX)
    target_link_libraries(NGE glx)
    target_link_libraries(NGE X11)
elseif (WIN32)
    target_link_libraries(NGE wgl)
endif ()


target_link_libraries(NGE stb_image)
target_link_libraries(NGE assimp)
target_link_libraries(NGE glm)
target_link_libraries(NGE freetype)

#set compiler warning level
target_compile_options(NGE PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wno-volatile>
        )