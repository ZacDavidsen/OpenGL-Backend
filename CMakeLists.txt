cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${CMAKE_SOURCE_DIR}/frontend/GLFW/glfw-3.2.1/")

add_subdirectory("${CMAKE_SOURCE_DIR}/OpenGlBackend/")

add_executable(graphics 
  frontend/main.cpp
  frontend/Entity.cpp
  frontend/window.cpp
)

target_include_directories (graphics
  PRIVATE "frontend/"
  PUBLIC "OpenGlBackend/"
  PUBLIC "OpenGlBackend/GLAD/include/"
)

target_link_libraries(graphics GLBackend glfw)

file(COPY "frontend/Resources/" DESTINATION "Resources/")
