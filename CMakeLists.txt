cmake_minimum_required(VERSION 3.10)

if (NOT UNIX AND NOT WIN32)
    message(FATAL_ERROR "Unsupported platform!")
endif()

# set project name
project("GraPra")


# ---------------------------------------------------------------------
# compiler options

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# setup for included dependencies
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
set(CMAKE_LINK_DIRECTORIES_BEFORE ON)

if(UNIX)
    add_compile_options(-pthread -march=native)
    add_compile_options(-Wall -pthread -pedantic -Wno-int-to-pointer-cast)
    set(CMAKE_CXX_FLAGS_DEBUG          "-g")

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

else()
    add_definitions(-DNOMINMAX)
    add_definitions(-D_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING) # wtf
    #with cmake version >3.8, CMAKE_INTERPROCEDURAL_OPTIMIZATION ON breaks windows dll export
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF) # disable lto to save time while linking

# GL preference
set(OpenGL_GL_PREFERENCE "GLVND")


# additional global includes

# traverse
add_subdirectory(common)
include_directories(common/cppgl/src)

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
endif()


include_directories(common)

add_subdirectory(external)

add_subdirectory(applications)
