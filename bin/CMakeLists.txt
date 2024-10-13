add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC tang)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})