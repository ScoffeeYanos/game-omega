file(GLOB_RECURSE APP_COMMON_SOURCES "game/*.cpp")

add_library(app_common STATIC ${APP_COMMON_SOURCES})

target_include_directories(app_common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/common
)