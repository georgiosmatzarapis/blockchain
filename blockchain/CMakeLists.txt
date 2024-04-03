set(Headers
 ${CMAKE_CURRENT_SOURCE_DIR}/include/User.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Transaction.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Block.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Common.hpp
)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/src/User.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Transaction.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Block.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Common.cpp
)

add_library(${PROJECT_NAME}_core_lib ${Headers} ${Sources})
target_include_directories(${PROJECT_NAME}_core_lib PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_core_lib PRIVATE ${PROJECT_NAME}_utils)