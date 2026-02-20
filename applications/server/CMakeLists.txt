file(GLOB_RECURSE SERVER_SOURCES "game/*.cpp")

set(TARGET server)

add_executable(${TARGET} ${SERVER_SOURCES} main.cpp)

target_include_directories(${TARGET}
        PRIVATE ./ ../
)

target_link_libraries(${TARGET}
        PRIVATE app_common
)