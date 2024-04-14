project(clang)
cmake_minimum_required(VERSION 3.6)

set(CMAKE_C_STANDARD 99)

add_compile_options("-Wpointer-to-int-cast")

# set param
set(IPC ipc)
set(FORK fork)
set(SOURCE .)

# pattern match
file(GLOB_RECURSE LIB "lib/*.c" "lib/*.h")

# add lib
include_directories("lib")

# IPC
add_executable(client ${IPC}/client.c)
add_executable(server ${IPC}/server.c)

# FORK
add_executable(clone ${FORK}/clone.c)
add_executable(jmp ${FORK}/jmp.c)

# MAIN
add_executable(oob_tcpsend01 ${SOURCE}/oob_tcpsend01.c ${LIB})
add_executable(oob_tcprecv01 ${SOURCE}/oob_tcprecv01.c ${LIB})

add_executable(udp_echoserver ${SOURCE}/udp_server.c ${LIB})
add_executable(udp_echoclient ${SOURCE}/udp_client.c ${LIB})

add_executable(tcp_server ${SOURCE}/tcp_server.c ${LIB})
add_executable(tcp_client ${SOURCE}/tcp_client.c ${LIB})

add_executable(iotimeout1 ${SOURCE}/io_connect.c ${LIB})

add_executable(tcpcli ${SOURCE}/tcpcli.c ${LIB})