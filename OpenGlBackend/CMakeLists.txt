cmake_minimum_required ( VERSION 3.0 )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(GLBackend STATIC
  Matrix.cpp
  Shader.cpp
  Model.cpp
  Camera.cpp
  GLManager.cpp
  ModelLoader.cpp
  ShaderLoader.cpp
  GLAD/src/glad.c
)

target_include_directories(GLBackend 
  PRIVATE "stbLibs/"
  PRIVATE "GLAD/include/"
)
