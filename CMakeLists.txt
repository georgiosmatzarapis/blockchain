cmake_minimum_required(VERSION 3.27)
project(${PROJECT_NAME} Blockchain LANGUAGES CXX
 VERSION 0.1.0
 DESCRIPTION "A blockchain implementation approach in C++"
 HOMEPAGE_URL "https://github.com/georgiosmatzarapis/blockchain")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests with google tests" ON)

add_subdirectory(utils)
add_subdirectory(blockchain)
add_subdirectory(app)

if (BUILD_TESTS)
 enable_testing()
 add_subdirectory(tests)
endif()