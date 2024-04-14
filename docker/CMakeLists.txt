cmake_minimum_required(VERSION 3.5)
project(docker)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. SOURCE_FILES)
aux_source_directory(udp SOURCE_UDP)
aux_source_directory(mutli SOURCE_MUTLI)
add_executable(docker ${SOURCE_FILES} ${SOURCE_UDP} ${SOURCE_MUTLI})