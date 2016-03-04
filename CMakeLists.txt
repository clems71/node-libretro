cmake_minimum_required(VERSION 3.0)
project(retro-api)

if (UNIX)
  add_definitions(-std=c++11)
endif()

add_library(${PROJECT_NAME} SHARED lib/core.cpp lib/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
