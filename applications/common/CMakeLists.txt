file(GLOB_RECURSE APP_COMMON_SOURCES "game/*.cpp" "comm/*.cpp")

add_library(app_common STATIC ${APP_COMMON_SOURCES})

find_package(Boost REQUIRED COMPONENTS system)

target_include_directories(app_common
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(app_common
        PUBLIC
        Boost::system
)