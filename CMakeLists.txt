cmake_minimum_required(VERSION 3.31)

project(ZucchiniTutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(Zucchini
        GIT_REPOSITORY https://github.com/AnarchoSystems/Zucchini.git
        GIT_TAG 9833510576ba9b02cffdaf334ff5287de70821bb)

block()
    # GoogleTest 1.12.1 still requests CMake 3.5 policy compatibility.
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0 AND
       (NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM OR
        CMAKE_POLICY_VERSION_MINIMUM VERSION_LESS 3.10))
        set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
    endif()
    FetchContent_MakeAvailable(Zucchini)
endblock()

enable_testing()

add_executable(ZucchiniTutorial main.cpp)

target_compile_options(ZucchiniTutorial PRIVATE -Wswitch-enum)

target_include_directories(ZucchiniTutorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

zucchinify(ZucchiniTutorial
            FEATURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/features
            FIXTURE TestTaskTracker)