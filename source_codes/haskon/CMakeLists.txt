project(HasKon_WebService)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(LIB_LOCAL "include/")
set(LIB_JSON "include/jsonlib/")

include_directories(${LIB_JSON} ${LIB_LOCAL})

file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDES "include/*.h")
file(GLOB JSON_INCLUDES "include/jsonlib/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES} ${JSON_INCLUDES})

find_package(Boost 1.54.0 REQUIRED system COMPONENTS filesystem regex)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} cpprest ssl crypto)




