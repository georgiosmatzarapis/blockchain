include(CTest)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/BlockTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/TransactionTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/UserTests.cpp
)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${Sources})
target_link_libraries(${PROJECT_NAME}_tests
 PRIVATE
 ${PROJECT_NAME}_core_lib
 GTest::GTest
 GTest::Main)

gtest_discover_tests(${PROJECT_NAME}_tests)