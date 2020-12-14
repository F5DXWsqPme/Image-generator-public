cmake_minimum_required(VERSION 3.16)

set(CURRENT_PROJECT_NAME ImageGenerator)

project(${CURRENT_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(PROJECT_SOURCES
  src/ext/volk/volk.h
  src/ext/volk/volk.c

  src/def.h

  src/utils/error.h
  src/utils/error.cpp
  src/utils/image.h
  src/utils/image.cpp

  src/vulkan_wrappers/buffer.h
  src/vulkan_wrappers/command_buffer.h
  src/vulkan_wrappers/command_pool.h
  src/vulkan_wrappers/compute_pipeline.h
  src/vulkan_wrappers/descriptor_pool.h
  src/vulkan_wrappers/descriptor_set_layout.h
  src/vulkan_wrappers/memory.h
  src/vulkan_wrappers/pipeline_cache.h
  src/vulkan_wrappers/pipeline_layout.h
  src/vulkan_wrappers/queue.h
  src/vulkan_wrappers/shader_module.h
  src/vulkan_wrappers/vulkan_application.h
  src/vulkan_wrappers/vulkan_validation.h
  src/vulkan_wrappers/buffer.cpp
  src/vulkan_wrappers/command_buffer.cpp
  src/vulkan_wrappers/command_pool.cpp
  src/vulkan_wrappers/compute_pipeline.cpp
  src/vulkan_wrappers/descriptor_pool.cpp
  src/vulkan_wrappers/descriptor_set_layout.cpp
  src/vulkan_wrappers/memory.cpp
  src/vulkan_wrappers/pipeline_cache.cpp
  src/vulkan_wrappers/pipeline_layout.cpp
  src/vulkan_wrappers/queue.cpp
  src/vulkan_wrappers/shader_module.cpp
  src/vulkan_wrappers/vulkan_application.cpp
  src/vulkan_wrappers/vulkan_validation.cpp
  src/vulkan_wrappers/fence.h
  src/vulkan_wrappers/fence.cpp
  src/vulkan_wrappers/semaphore.h
  src/vulkan_wrappers/semaphore.cpp
  src/vulkan_wrappers/event.h
  src/vulkan_wrappers/event.cpp

  src/math/matr.h
  src/math/vec.h
  src/math/ray.h
  src/math/matr.cpp
  src/math/vec.cpp
  src/math/ray.cpp

  src/scene/aabb.h
  src/scene/cam.h
  src/scene/environment.h
  src/scene/scene_data.h
  src/scene/kd_tree_node_data.h
  src/scene/params.h
  src/scene/grid.h
  src/scene/kd_tree.h
  src/scene/material.h
  src/scene/scene.h
  src/scene/shape.h
  src/scene/texture.h
  src/scene/triangle.cpp
  src/scene/aabb.cpp
  src/scene/cam.cpp
  src/scene/environment.cpp
  src/scene/grid.cpp
  src/scene/kd_tree.cpp
  src/scene/material.cpp
  src/scene/scene.cpp
  src/scene/shape.cpp
  src/scene/triangle.cpp

  src/render/base_render.h
  src/render/render.h
  src/render/render.cpp

  src/render/cpu_render/cpu_render.h
  src/render/cpu_render/cpu_render.cpp
  src/render/cpu_render/cpu_hdr.h
  src/render/cpu_render/cpu_hdr.cpp
  src/render/cpu_render/trace.h
  src/render/cpu_render/trace.cpp

  src/render/vulkan_render/vulkan_render.h
  src/render/vulkan_render/vulkan_render.cpp

  src/scene_loader/scene_loader.h
  src/scene_loader/scene_loader.cpp)

add_executable(${CURRENT_PROJECT_NAME}
  ${PROJECT_SOURCES}
  src/main.cpp)

target_include_directories(${CURRENT_PROJECT_NAME} PRIVATE src)

add_subdirectory(src/ext/volk)
target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE volk_headers)

find_package(Vulkan)

IF (Vulkan_FOUND)
  message(STATUS "Vulkan include directory: ${Vulkan_INCLUDE_DIRS}")
  message(STATUS "Vulkan library: ${Vulkan_LIBRARY}")
  target_include_directories(${CURRENT_PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
ELSE (Vulkan_FOUND)
  message(FATAL_ERROR "Vulkan not found.")
ENDIF (Vulkan_FOUND)

IF (DEFINED ENV{VULKAN_SDK})
  IF(WIN32)
    set(GLSL_COMPILER $ENV{VULKAN_SDK}/Bin/glslangValidator.exe)
  ELSE(WIN32)
    set(GLSL_COMPILER $ENV{VULKAN_SDK}/bin/glslangValidator)
  ENDIF(WIN32)
ELSEIF(DEFINED VULKAN_SDK)
  IF(WIN32)
    set(GLSL_COMPILER ${VULKAN_SDK}/Bin/glslangValidator.exe)
  ELSE(WIN32)
    set(GLSL_COMPILER ${VULKAN_SDK}/bin/glslangValidator)
  ENDIF(WIN32)
ENDIF(DEFINED ENV{VULKAN_SDK})

IF (DEFINED GLSL_COMPILER)
  message(STATUS "GLSL compiler: ${GLSL_COMPILER}")

  file(GLOB SHADERS_SOURCES "shaders/*.comp")
  message(STATUS "Shaders ${SHADERS_SOURCES}")

  set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
  set(SHADERS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders-build)

  FOREACH(SHADER ${SHADERS_SOURCES})
    file(RELATIVE_PATH SHADER_RELATIVE_PATH ${SHADERS_DIR} ${SHADER})
    message(STATUS "Compile command: ${GLSL_COMPILER} -V ${SHADER} -o ${SHADERS_BUILD_DIR}/${SHADER_RELATIVE_PATH}.spv")
    execute_process(COMMAND ${GLSL_COMPILER} -V ${SHADER} -o ${SHADERS_BUILD_DIR}/${SHADER_RELATIVE_PATH}.spv RESULT_VARIABLE EXIT_CODE)

    IF(${EXIT_CODE} EQUAL 0)
      message(STATUS "Shader successfully builded.")
    ELSE(${EXIT_CODE} EQUAL 0)
      message(STATUS "Shader not builded.")
    ENDIF(${EXIT_CODE} EQUAL 0)

  ENDFOREACH(SHADER)
ELSE(DEFINED GLSL_COMPILER)
  message(STATUS "GLSL compiler not found. Shaders not compiled.")
ENDIF(DEFINED GLSL_COMPILER)

find_package(Boost)

target_include_directories(${CURRENT_PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

message(STATUS "Boost include directory: ${Boost_INCLUDE_DIRS}")

add_executable (Tests-run
  ${PROJECT_SOURCES}
  tests/tests_main.cpp
  tests/cpu_and_gpu_render_should_get_equal.cpp
  tests/cpu_and_gpu_render_with_scene_loader_test.cpp
  tests/matrix_tests.cpp
  tests/vec_tests.cpp)

target_include_directories(Tests-run PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(Tests-run PRIVATE src)

target_link_libraries(Tests-run PRIVATE volk_headers)

IF (Vulkan_FOUND)
  target_include_directories(Tests-run PRIVATE ${Vulkan_INCLUDE_DIRS})
ELSE (Vulkan_FOUND)
  message(FATAL_ERROR "Vulkan not found.")
ENDIF (Vulkan_FOUND)