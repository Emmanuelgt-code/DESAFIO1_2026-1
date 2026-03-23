cmake_minimum_required(VERSION 3.16)

project(DESAFIO_1git_hub LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DESAFIO_1git_hub main.cpp
    funciones.cpp
    funciones.h)

include(GNUInstallDirs)
install(TARGETS DESAFIO_1git_hub
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
