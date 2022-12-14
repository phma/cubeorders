project(cubeorders)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Quadlods REQUIRED)
find_package(GMPXX REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(Threads)

add_executable(cubeorders cubeorders.cpp order.cpp threads.cpp random.cpp)
target_link_libraries(cubeorders ${CMAKE_THREAD_LIBS_INIT} ${GMP_LIBRARY} ${GMPXX_LIBRARIES} ${Quadlods_LIBRARY} ${Boost_LIBRARIES})

set(CUBEORDERS_MAJOR_VERSION 0)
set(CUBEORDERS_MINOR_VERSION 2)
set(CUBEORDERS_PATCH_VERSION 0)
set(CUBEORDERS_VERSION ${CUBEORDERS_MAJOR_VERSION}.${CUBEORDERS_MINOR_VERSION}.${CUBEORDERS_PATCH_VERSION})
set(COPY_YEAR 2022)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${Quadlods_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
configure_file (config.h.in config.h)

set(CPACK_PACKAGE_VERSION_MAJOR ${CUBEORDERS_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${CUBEORDERS_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${CUBEORDERS_PATCH_VERSION})
set(CPACK_SOURCE_IGNORE_FILES /\\\\.git;.*~)
include(CPack)
