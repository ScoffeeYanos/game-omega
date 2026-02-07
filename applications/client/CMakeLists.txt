# glob for source files
file(GLOB_RECURSE CORE_SOURCES "core/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "game/*.cpp")

set(TARGET aaa)

add_executable(${TARGET} ${CORE_SOURCES} ${GAME_SOURCES} main.cpp)

# output in source
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# define base path for mesh/image lookups
target_compile_definitions(${TARGET}
    PRIVATE -DEXECUTABLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

# handle dependencies

# OpenGL
find_package(OpenGL REQUIRED)

target_include_directories(${TARGET}
    PRIVATE ${OPENGL_INCLUDE_DIRS} ./
)
target_link_libraries(${TARGET}
    PRIVATE ${OPENGL_LIBRARIES}
)

# FFmpeg via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libswscale libavutil)

target_include_directories(${TARGET}
    PRIVATE ${FFMPEG_INCLUDE_DIRS}
)
target_link_libraries(${TARGET}
    PRIVATE ${FFMPEG_LIBRARIES}
)

# common libs
target_link_libraries(${TARGET}
    PRIVATE cppgl nlohmann_json soloud
)