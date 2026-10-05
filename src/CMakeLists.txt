# src/CMakeLists.txt

# Streamline the build config
add_executable(GEP26S "main.cpp" "game.cpp" "input/input_system.cpp" "time/time_manager.cpp")

target_compile_features(GEP26S PRIVATE cxx_std_23)
target_link_libraries(GEP26S PRIVATE 
	SDL3::SDL3
	SDL3_image::SDL3_image
)

set(ASSETS_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/assets" CACHE STRING "Input directory of in-game assets")

add_custom_target(CopyAssets COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${ASSETS_INPUT_DIRECTORY} $<TARGET_FILE_DIR:GEP26S>/assets)

add_dependencies(GEP26S CopyAssets)