cmake_minimum_required(VERSION 3.26)
project(${PROJECT_NAME} blockchain)

set(CMAKE_CXX_STANDARD 23)

set(Headers
				include/Block.hpp
				include/Blockchain.hpp
)

set(Sources
				main.cpp
				src/Block.cpp
				src/Blockchain.cpp
)

add_executable(${PROJECT_NAME}
				${Headers} ${Sources})
