cmake_minimum_required (VERSION 3.30)

project("This Is Not Touhou")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../out/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../out/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../out/data")
set(CMAKE_PDB_OUTPUT_DIRECTORY "../tmp")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(
		/fsanitize=address
		/W3 /WX /EHsc
		/wd4455 #literal-suffix
	)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(
		-fsanitize=address
		-Wall -Werror
		-Wno-reserved-user-defined-literal
		-Wno-user-defined-literals
		-Wno-literal-suffix
	)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
		add_link_options("/DEBUG")
	else()
		set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
	endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
	endif()
endif()

add_compile_options(-std=c++26)

set(CMAKE_SYSTEM_PROCESSOR x86)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CMAKE_SYSTEM_VERSION 6.12.0)
	if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options(-pthread)
		add_link_options(-pthread)
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CMAKE_SYSTEM_VERSION 5.1) #XP SP3 x86
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	else()
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
			set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)
			set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
			set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
			set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
		endif()
		add_compile_options(-DWIN32 -D_WIN32)
	endif()
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(core "Core") #core objects & functionality
set(zip "Quarc") #resource extractor
set(pattern "Pattern.dat") #patterns
set(stager "StageManager") #stage manager
set(game "Game") #main game
set(rbotbase "ReplayBot") #replay bot backend
set(rbotexe "ReplayBot") #replay bot frontend

add_library(${zip} SHARED "zip.cpp")
add_library(${core} SHARED "internal.cpp" "core.cpp" "audioStream.cpp" "nodes.cpp")
# add_library(${pattern} SHARED "patterns.cpp")
# add_library(${stager} SHARED "registers.cpp" "eventHandler.cpp" "scriptManager.cpp" "stageManager.cpp" "stage.cpp")
add_executable(${game} "game.cpp")
# add_library(${rbotbase} SHARED "inject.cpp" "replayBase.cpp")
# add_executable(${rbotexe} "replayUI.cpp" "replayBot.cpp")

target_link_libraries(${core} PUBLIC SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_link_libraries(${zip} PUBLIC ${core})
# target_link_libraries(${pattern} PRIVATE ${core})
# target_link_libraries(${stager} PRIVATE ${core} ${pattern})
target_link_libraries(${game} PRIVATE ${core} ${zip} )#${stager})
#target_link_libraries(${rbotbase} PRIVATE ${core} ${stager})
#target_link_libraries(${rbotexe} PRIVATE ${rbotbase})