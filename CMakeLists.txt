cmake_minimum_required(VERSION 3.3)

if(	CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND
	NOT CMAKE_BUILD_TYPE				   AND
	NOT CMAKE_CONFIGURATION_TYPES
)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(CPMe LANGUAGES C DESCRIPTION "CP/M emulator")
find_package(Z80 REQUIRED)
add_executable(cpme "${CMAKE_CURRENT_SOURCE_DIR}/sources/cpme.c")
target_link_libraries(cpme PRIVATE Z80)
