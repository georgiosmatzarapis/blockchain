set(Headers
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Logger.hpp
)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp
)

add_library(${PROJECT_NAME}_utils ${Headers} ${Sources})
target_include_directories(${PROJECT_NAME}_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)