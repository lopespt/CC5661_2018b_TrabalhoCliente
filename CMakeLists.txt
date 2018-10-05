cmake_minimum_required(VERSION 3.0.0)
project(solucoes VERSION 0.1.0)

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})
include_directories(".")

add_executable(solucoes main.cpp)
target_link_libraries(solucoes ${CURL_LIBRARIES})

