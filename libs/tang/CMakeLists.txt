set(LIBRARY_NAME tang)

file(GLOB_RECURSE SOURCES CONFIGURE src/*.cpp)
add_library(${LIBRARY_NAME} ${SOURCES}
        include/tang/token.hpp
        src/token.cpp)
target_include_directories(${LIBRARY_NAME} PUBLIC include)