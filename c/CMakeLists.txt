cmake_minimum_required(VERSION 3.0)

project(advance)

add_library(util STATIC util/log.h util/log.c)
add_library(detail STATIC detail/struct.h detail/struct.c detail/thread.c detail/server.c main/main.c detail/variable.c detail/memory.c detail/symbol.c)

add_executable(main main/main.c)