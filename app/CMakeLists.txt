set(Sources
 ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
)

add_executable(${PROJECT_NAME}_app ${Sources})
target_link_libraries(${PROJECT_NAME}_app PRIVATE ${PROJECT_NAME}_core_lib)