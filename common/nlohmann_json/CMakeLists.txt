
add_library(nlohmann_json
    include/nlohmann/convert.hpp
    include/nlohmann/convert_glm.hpp
    include/nlohmann/json.hpp
    include/nlohmann/json_fwd.hpp

    convert_glm.cpp
)

target_include_directories(nlohmann_json
    INTERFACE include/
    PRIVATE include/nlohmann/
)

# Link cppgl publicly since its headers may be included by the convert headers
target_link_libraries(nlohmann_json
    PUBLIC cppgl
)
