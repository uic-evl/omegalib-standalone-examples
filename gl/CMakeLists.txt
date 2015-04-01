cmake_minimum_required(VERSION 2.8.1)
project(myohello)

find_package(Omegalib)

include_directories(${OMEGA_INCLUDE_DIRS})        
add_executable(myohello ohello.cpp)
target_link_libraries(myohello ${OMEGA_LIB})

