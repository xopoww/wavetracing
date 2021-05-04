cmake_minimum_required(VERSION 3.14)

project(wavetracing LANGUAGES CXX)


add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE 
    "${PROJECT_SOURCE_DIR}/main.cpp"
)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}")


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE WAVETRACING_DEBUG=1)
endif()