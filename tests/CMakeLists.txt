include(CTest)

set(Mocks
 ${CMAKE_CURRENT_SOURCE_DIR}/mocks/OpenSslApiMock.hpp
)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/BlockTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/TransactionTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/UserTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/UtilsTests.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/CommonTests.cpp
)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${Mocks} ${Sources})
target_include_directories(${PROJECT_NAME}_tests PRIVATE
 ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

target_link_libraries(${PROJECT_NAME}_tests
 PRIVATE
 ${PROJECT_NAME}_core_lib
 ${PROJECT_NAME}_utils
 GTest::gmock_main)

gtest_discover_tests(${PROJECT_NAME}_tests)