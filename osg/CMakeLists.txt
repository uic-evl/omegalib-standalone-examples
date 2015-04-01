cmake_minimum_required(VERSION 2.8.1)
project(bcc)

find_package(Omegalib)

include_directories(${OMEGA_INCLUDE_DIRS})        

add_executable(bcc BCC_OSG.cpp)
target_link_libraries(bcc ${OMEGA_LIB} ${OMEGA_TOOLKIT_LIB} ${OMEGA_OSG_LIB} ${OSG_LIBS}) 
