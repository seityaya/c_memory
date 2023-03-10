#Author                 : Seityagiya Terlekchi
#Contacts               : seityaya@ukr.net
#Creation Date          : 2022.12
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2022-2023 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 11)
add_definitions(-std=c11)

add_compile_options(-Wall -Wfatal-errors -Wconversion)

set(VERSION 0.8)
set(PROJECT_NAME memory_test)
project(${PROJECT_NAME})

add_definitions(-DYAYA_MEMORY_STATS_USE=1)
add_definitions(-DYAYA_MEMORY_STATS_OFF=0)
add_definitions(-DYAYA_MEMORY_MACRO_DEF=1)
add_definitions(-DYAYA_MEMORY_STATS_GLOBAL=1)

add_executable(
    ${PROJECT_NAME}
    main.c
    )

add_subdirectory(${CMAKE_SOURCE_DIR}/../lib/ yaya_memory)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/)
target_link_libraries(${PROJECT_NAME} yaya_memory)
