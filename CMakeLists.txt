cmake_minimum_required (VERSION 3.8)

project (mem2pix LANGUAGES CXX)
set (APP_VERSION "1.1")

list (APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
)

include (compiler_runtime)
include (msvc_winxp_support)

set (DIR_EXTERNAL ${CMAKE_SOURCE_DIR}/external)
set (DIR_SOURCES ${CMAKE_SOURCE_DIR}/sources)
set (DIR_COMMON_OUTPUT 
	${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_BUILD_TYPE}
)

# include SDL2 library subproject
add_subdirectory (
	${DIR_EXTERNAL}/sdl2
	${DIR_COMMON_OUTPUT}
)

file (GLOB FILE_SOURCES ${DIR_SOURCES}/*.cpp)
file (GLOB FILE_HEADERS ${DIR_SOURCES}/*.h)

if (MSVC)
	file (GLOB FILE_SOURCES_PLATFORM ${DIR_SOURCES}/windows/*.cpp)
	file (GLOB FILE_HEADERS_PLATFORM ${DIR_SOURCES}/windows/*.h)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	file (GLOB FILE_SOURCES_PLATFORM ${DIR_SOURCES}/linux/*.cpp)
	file (GLOB FILE_HEADERS_PLATFORM ${DIR_SOURCES}/linux/*.h)
endif ()
list (APPEND FILE_SOURCES ${FILE_SOURCES_PLATFORM})
list (APPEND FILE_HEADERS ${FILE_HEADERS_PLATFORM})
add_executable (${PROJECT_NAME} ${FILE_HEADERS} ${FILE_SOURCES})

if (MSVC)
	target_include_directories (${PROJECT_NAME} PRIVATE
		${DIR_SOURCES}/windows
	)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_include_directories (${PROJECT_NAME} PRIVATE
		${DIR_SOURCES}/linux
	)
endif ()

target_include_directories (${PROJECT_NAME} PRIVATE
	${DIR_SOURCES}
	${DIR_EXTERNAL}/sdl2/include
)

# pass through app version to code
target_compile_definitions(${PROJECT_NAME} PRIVATE
	APP_VERSION="${APP_VERSION}"
)

if (MSVC)
	# disable CRT warnings
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

	# enable WinXP support
	enable_winxp_support(${PROJECT_NAME})

	# disable D3D11 render for SDL2 library (for WinXP compatibility)
	set (HAVE_D3D11_H FALSE CACHE INTERNAL "")

	# set PSAPI version (for WinXP compatibility)
	add_compile_definitions(PSAPI_VERSION=1)

	# include PSAPI static library
	target_link_libraries(${PROJECT_NAME} psapi)
endif ()

# set static runtime
set_compiler_runtime (${PROJECT_NAME} STATIC)

# link SDL2 library to project
target_link_libraries (${PROJECT_NAME} SDL2)

set_target_properties (${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
    RUNTIME_OUTPUT_DIRECTORY ${DIR_COMMON_OUTPUT}
)

set_target_properties (${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

