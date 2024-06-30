cmake_minimum_required(VERSION 3.27)

project(FileSorter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Dependencies
block(SCOPE_FOR VARIABLES)

    include(FetchContent)

    set(BOOST_INCLUDE_LIBRARIES program_options)
    set(BUILD_SHARED_LIBS OFF)

    FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.85.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(
        Boost
    )

endblock()

set(HEADERS 
    
)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS})
target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        Boost::program_options
)
