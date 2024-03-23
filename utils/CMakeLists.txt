set(Headers
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Logger.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/Hmac.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/IOpenSslApi.hpp
 ${CMAKE_CURRENT_SOURCE_DIR}/include/OpenSslApi.hpp
)

set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/Hmac.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/OpenSslApi.cpp
)

find_package(OpenSSL REQUIRED)

add_library(${PROJECT_NAME}_utils ${Headers} ${Sources})
target_include_directories(${PROJECT_NAME}_utils PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}_utils PRIVATE OpenSSL::SSL)