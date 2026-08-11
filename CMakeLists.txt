cmake_minimum_required (VERSION 3.30)

project("This Is Not Touhou")

set(debug "-g -O0")
set(release "-O3 -DNDEBUG")
set(windows "")
set(linux "")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "-std=c++26 ${debug} ${linux} -fsanitize=address -Wall -Werror -Wno-reserved-user-defined-literal -Wno-user-defined-literals -Wno-literal-suffix -Wno-c23-extensions")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../out/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../out/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../out/data")
set(CMAKE_PDB_OUTPUT_DIRECTORY "../tmp")

set(core "Core") #core objects & functionality
set(zip "Quarc") #resource extractor
set(pattern "Pattern.dat") #patterns
set(stager "StageManager") #stage manager
set(game "Game") #main game
# set(rbotbase "ReplayBot") #replay bot backend
# set(rbotexe "ReplayBot") #replay bot frontend

add_library(${zip} SHARED "zip.cpp")
add_library(${core} SHARED "internal.cpp" "core.cpp" "audioStream.cpp" "nodes.cpp")
# add_library(${pattern} SHARED "patterns.cpp")
# add_library(${stager} SHARED "registers.cpp" "eventHandler.cpp" "scriptManager.cpp" "stageManager.cpp" "stage.cpp")
add_executable(${game} "game.cpp")
# add_library(${rbotbase} SHARED "inject.cpp" "replayBase.cpp")
# add_executable(${rbotexe} "replayUI.cpp" "replayBot.cpp")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(${core} PUBLIC SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_link_libraries(${zip} PUBLIC ${core})
# target_link_libraries(${pattern} PRIVATE ${core})
# target_link_libraries(${stager} PRIVATE ${core} ${pattern})
target_link_libraries(${game} PRIVATE ${core} ${zip} )#${stager})
#target_link_libraries(${rbotbase} PRIVATE ${core} ${stager})
#target_link_libraries(${rbotexe} PRIVATE ${rbotbase})