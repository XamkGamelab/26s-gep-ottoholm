# src/CMakeLists.txt

# Streamline the build config
add_executable(GEP26S "main.cpp" "game.cpp")
target_compile_features(GEP26S PRIVATE cxx_std_23)

target_link_libraries(GEP26S PRIVATE SDL3::SDL3)

