cmake_minimum_required(VERSION 3.10)

project(s-socket)

add_library(s-socket)

target_include_directories(s-socket INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include>)
set_target_properties(s-socket PROPERTIES PUBLIC_HEADER s-socket.h)

# Detect target
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows" )
    target_sources(s-socket PRIVATE s-socket-win.c)
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    target_sources(s-socket PRIVATE s-socket-linux.c)
else()
    message(FATAL_ERROR "Unsupported Platform: ${CMAKE_SYSTEM_NAME}")
endif()


install(TARGETS s-socket EXPORT s-socket
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include)

install(EXPORT s-socket
    FILE s-socketTargets.cmake
    NAMESPACE nyarmith::
    DESTINATION share/cmake/s-socket)
