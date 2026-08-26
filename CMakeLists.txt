
# ######################################################################## #
# File:     /CMakeLists.txt
#
# Purpose:  Top-level CMake lists file for woad
#
# Created:  15th August 2026
# Updated:  27th August 2026
#
# ######################################################################## #


# ##########################################################
# CMake

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# require out-of-source builds
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")

	message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# directory for CMake specific extensions and source files.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# ##########################################################
# project

project(woad
	DESCRIPTION "Minimal ANSI terminal colour codes"
	HOMEPAGE_URL "https://github.com/synesissoftware/woad"
	LANGUAGES C
)

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

# handle version number
set(RX_PROJ_TAG "${PROJECT_NAME_UPPER}")
set(RX_WS "[ \t]")
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME_LOWER}/${PROJECT_NAME_LOWER}.h" _header_file)
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+_?${RX_PROJ_TAG}_VER_MAJOR${RX_WS}+([0-9]+)" MAJOR_DUMMY ${_header_file})
set(_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+_?${RX_PROJ_TAG}_VER_MINOR${RX_WS}+([0-9]+)" MINOR_DUMMY ${_header_file})
set(_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "#${RX_WS}*define${RX_WS}+_?${RX_PROJ_TAG}_VER_(PATCH|REVISION)${RX_WS}+([0-9]+)" PATCH_DUMMY ${_header_file})
set(_VERSION_PATCH ${CMAKE_MATCH_2})

# set project version number here
set(PROJECT_VERSION_MAJOR ${_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${_VERSION_PATCH})
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# adhere strictly to C standards plus extensions. These are actually
# unused by the (header-only) library; they merely state our intention
# for tests and examples.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

	set(_WOAD_IS_TOPLEVEL TRUE)
else()

	set(_WOAD_IS_TOPLEVEL FALSE)
endif()

option(BUILD_EXAMPLES "Build examples" ${_WOAD_IS_TOPLEVEL})
option(BUILD_TESTING "Build tests" ${_WOAD_IS_TOPLEVEL})
option(MSVC_USE_MT "Use static MSVC runtime (/MT)" OFF)

if(MSVC)

	if(MSVC_VERSION LESS 1930)

		set(CMAKE_C_STANDARD 90)
	endif()

	if(MSVC_USE_MT)

		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
else()

	if(MSVC_USE_MT)

		# Present so prepare_cmake.sh can pass -DMSVC_USE_MT without unused-
		# variable warnings on non-MSVC toolchains.
	endif()
endif()


# ##########################################################
# OS discrimination

if(UNIX)

	set(_BUILD_AS_UNIX 1)

	message(STATUS "Operating system Unix")
else()

	if(WIN32)

		set(_BUILD_AS_WIN32 1)

		message(STATUS "Operating system Windows")
	else()

		message(STATUS "Operating system unknown")
	endif()
endif()


# ##########################################################
# includes

include(BuildType)
include(CMakePackageConfigHelpers)
if(BUILD_TESTING)

	include(CTest)
endif()
include(GNUInstallDirs)


# ##########################################################
# library

add_library(woad INTERFACE)
add_library(woad::woad ALIAS woad)

target_include_directories(woad
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(WOAD_COMPILE_OPTIONS
	$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
	$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
	$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
	$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-Wpedantic>
	$<$<C_COMPILER_ID:MSVC>:/W4>
	$<$<C_COMPILER_ID:MSVC>:/WX>
)


# ##########################################################
# examples

if(BUILD_EXAMPLES)

	message(STATUS "enabled building of examples ...")

	add_executable(colour examples/colour.c)
	target_link_libraries(colour PRIVATE woad::woad)
	target_compile_options(colour PRIVATE ${WOAD_COMPILE_OPTIONS})
	set_target_properties(colour PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
	)

	add_executable(version examples/version.c)
	target_link_libraries(version PRIVATE woad::woad)
	target_compile_options(version PRIVATE ${WOAD_COMPILE_OPTIONS})
	set_target_properties(version PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
	)
else()

	message(STATUS "disabled building of examples - define BUILD_EXAMPLES to enable")
endif()


# ##########################################################
# tests

if(BUILD_TESTING)

	message(STATUS "enabled building of tests ...")

	add_executable(test.unit.codes test/unit/test.unit.codes.c)
	target_link_libraries(test.unit.codes PRIVATE woad::woad)
	target_compile_options(test.unit.codes PRIVATE ${WOAD_COMPILE_OPTIONS})
	set_target_properties(test.unit.codes PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
	)
	add_test(NAME unit.codes COMMAND test.unit.codes)
	set_tests_properties(unit.codes PROPERTIES TIMEOUT 30)

	add_executable(test.unit.version test/unit/test.unit.version.c)
	target_link_libraries(test.unit.version PRIVATE woad::woad)
	target_compile_options(test.unit.version PRIVATE ${WOAD_COMPILE_OPTIONS})
	set_target_properties(test.unit.version PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
	)
	add_test(NAME unit.version COMMAND test.unit.version)
	set_tests_properties(unit.version PROPERTIES TIMEOUT 30)
else()

	message(STATUS "disabled building of tests - define BUILD_TESTING to enable")
endif()


# ##########################################################
# pkg-config

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/woad.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/woad.pc"
	@ONLY
)


# ##########################################################
# install / export

set(EXPORT_NAME ${PROJECT_NAME_LOWER})

install(TARGETS woad
	EXPORT project-targets
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/woad"
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/woad.pc"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

install(EXPORT project-targets
	NAMESPACE "${PROJECT_NAME}::"
	FILE "${EXPORT_NAME}-targets.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXPORT_NAME}
)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/${EXPORT_NAME}-config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${EXPORT_NAME}-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXPORT_NAME}
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/cmake/${EXPORT_NAME}-config-version.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/${EXPORT_NAME}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/${EXPORT_NAME}-config-version.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXPORT_NAME}
)


# ##########################################################
# completion

message(NOTICE "Generating CMake build scripts for ${PROJECT_NAME} ${PROJECT_VERSION}, for C${CMAKE_C_STANDARD}, with build type ${CMAKE_BUILD_TYPE}")


# ############################## end of file ############################# #
