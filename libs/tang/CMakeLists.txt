set(LIBRARY_NAME tang)

file(GLOB_RECURSE SOURCES CONFIGURE src/*.cpp)
add_library(${LIBRARY_NAME} ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC include)