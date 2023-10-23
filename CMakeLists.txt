cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
project(${PROJECT_NAME} blockchain)

set(CMAKE_CXX_STANDARD 23)

set(Headers
  include/Block.hpp
  include/TransactionPayload.hpp
)

set(Sources
  main.cpp
  src/Block.cpp
  src/TransactionPayload.cpp
)

add_executable(${PROJECT_NAME}
  ${Headers} ${Sources})

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++23)