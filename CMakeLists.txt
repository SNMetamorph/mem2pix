cmake_minimum_required (VERSION 3.3)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if(POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW)
endif()

if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

project(mem2pix LANGUAGES CXX)
include(CompilerRuntime)
include(SupportWinXP)

set(DIR_EXTERNAL ${CMAKE_SOURCE_DIR}/external)
set(DIR_SOURCES ${CMAKE_SOURCE_DIR}/sources)
set(DIR_COMMON_OUTPUT 
	${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/
)
message(STATUS "Common binaries output directory: ${DIR_COMMON_OUTPUT}")

# add source files
file(GLOB FILE_SOURCES ${DIR_SOURCES}/*.cpp)

if(WIN32)
	file(GLOB FILE_SOURCES_PLATFORM ${DIR_SOURCES}/windows/*.cpp)
elseif(UNIX)
	file(GLOB FILE_SOURCES_PLATFORM ${DIR_SOURCES}/linux/*.cpp)
endif()

list(APPEND FILE_SOURCES ${FILE_SOURCES_PLATFORM})
add_executable(${PROJECT_NAME} ${FILE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${DIR_SOURCES}
	${DIR_EXTERNAL}/sdl2/include
)

if(WIN32)
	target_include_directories(${PROJECT_NAME} PRIVATE
		${DIR_SOURCES}/windows
	)
elseif(UNIX)
	target_include_directories(${PROJECT_NAME} PRIVATE
		${DIR_SOURCES}/linux
	)
endif()

# disable SDL2 modules which are not used
set(SDL_AUDIO OFF CACHE BOOL "")
set(SDL_FILESYSTEM OFF CACHE BOOL "")
set(SDL_FILE OFF CACHE BOOL "")
set(SDL_TIMERS OFF CACHE BOOL "")
set(SDL_THREADS OFF CACHE BOOL "")
set(SDL_ATOMIC OFF CACHE BOOL "")
set(SDL_CPUINFO OFF CACHE BOOL "")

if(WIN32)
	# disable DirectX in SDL2 (for Windows XP compatibility)
	set(DIRECTX OFF CACHE BOOL "")
endif()

# include SDL2 library subproject
add_subdirectory(
	${DIR_EXTERNAL}/sdl2
	${DIR_COMMON_OUTPUT}
)

if(MSVC)
	# disable CRT warnings
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

	# enable WinXP support
	enable_winxp_support(${PROJECT_NAME})

	# set PSAPI version (for WinXP compatibility)
	add_compile_definitions(PSAPI_VERSION=1)

	# include PSAPI static library
	target_link_libraries(${PROJECT_NAME} psapi)

	# multithreaded build flag
	add_compile_options(/MP) 
endif()

# set static runtime
set_compiler_runtime(${PROJECT_NAME} STATIC)

# link SDL2 library to project
target_link_libraries(${PROJECT_NAME} SDL2)

if(MSVC)
	set_target_properties(${PROJECT_NAME} SDL2 PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${DIR_COMMON_OUTPUT}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${DIR_COMMON_OUTPUT}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${DIR_COMMON_OUTPUT}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${DIR_COMMON_OUTPUT}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DIR_COMMON_OUTPUT}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR_COMMON_OUTPUT}
	)
else()
	set_target_properties(${PROJECT_NAME} SDL2 PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
		LIBRARY_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
		RUNTIME_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
