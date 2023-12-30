set(Headers
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Block.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/TransactionPayload.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/User.hpp
)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Block.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/TransactionPayload.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/User.cpp
)

add_library(${PROJECT_NAME}_lib ${Headers} ${Sources})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)