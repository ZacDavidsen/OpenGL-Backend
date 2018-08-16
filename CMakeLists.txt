cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_SOURCE_DIR}/frontend/GLFW/glfw-3.2.1/")

add_subdirectory("${CMAKE_SOURCE_DIR}/OpenGlBackend/")

add_executable(graphics 
  frontend/main.cpp
  frontend/Entity.cpp
  frontend/window.cpp
  frontend/State.cpp
)

target_include_directories (graphics
  PRIVATE "frontend/"
  PUBLIC "OpenGlBackend/"
  PUBLIC "OpenGlBackend/GLAD/include/"
)

target_link_libraries(graphics GLBackend glfw)

add_custom_command(TARGET graphics
  POST_BUILD
  COMMAND [ -d "${CMAKE_BINARY_DIR}/Resources" ] || mkdir "${CMAKE_BINARY_DIR}/Resources"
  COMMAND cp --verbose --update --preserve=mode,ownership,timestamps --recursive "${CMAKE_SOURCE_DIR}/frontend/Resources/*" -t "${CMAKE_BINARY_DIR}/Resources"
  COMMENT "Copying resources to build directory..."
)
