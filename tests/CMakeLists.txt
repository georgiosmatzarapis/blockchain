include(CTest)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/block_tests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/transaction_tests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/user_tests.cpp
)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${Sources})
target_link_libraries(${PROJECT_NAME}_tests
 PRIVATE
 ${PROJECT_NAME}_lib
 GTest::GTest
 GTest::Main)

gtest_discover_tests(${PROJECT_NAME}_tests)