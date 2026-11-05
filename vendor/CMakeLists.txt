# vendor/CMakeLists.txt
include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Enable FetchContent logging")

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.4.16 # fa2c02bb6e21974a89ea9824bc53c9932abe5f9c
	GIT_SHALLOW ON
	GIT_PROGRESS ON
)

FetchContent_Declare(
	SDL_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG release-3.4.6 # f661fa1ad24ab1b81e43662532f9a6a9fcf67ea6
	GIT_SHALLOW ON
	GIT_PROGRESS ON	
	GIT_SUBMODULES "external/libpng" "external/zlib"
)

set(SDLIMAGE_ANI OFF CACHE BOOL "Support loading ANI animations" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "Support loading AVIF images" FORCE)
set(SDLIMAGE_BMP OFF CACHE BOOL "Support loading BMP images" FORCE)
set(SDLIMAGE_GIF OFF CACHE BOOL "Support loading GIF images" FORCE)
set(SDLIMAGE_JPG ON CACHE BOOL "Support loading JPEG images" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "Support loading JXL images" FORCE)
set(SDLIMAGE_LBM OFF CACHE BOOL "Support loading LBM images" FORCE)
set(SDLIMAGE_PCX OFF CACHE BOOL "Support loading PCX images" FORCE)
set(SDLIMAGE_PNG ON CACHE BOOL "Support loading PNG images" FORCE)
set(SDLIMAGE_PNM OFF CACHE BOOL "Support loading PNM images" FORCE)
set(SDLIMAGE_QOI OFF CACHE BOOL "Support loading QOI images" FORCE)
set(SDLIMAGE_SVG ON CACHE BOOL "Support loading SVG images" FORCE)
set(SDLIMAGE_TGA ON CACHE BOOL "Support loading TGA images" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "Support loading TIFF images" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "Support loading WEBP images" FORCE)
set(SDLIMAGE_XCF OFF CACHE BOOL "Support loading XCF images" FORCE)
set(SDLIMAGE_XPM OFF CACHE BOOL "Support loading XPM images" FORCE)
set(SDLIMAGE_XV OFF CACHE BOOL "Support loading XV images" FORCE)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.3 # 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed
	GIT_SHALLOW ON
	GIT_PROGRESS ON
)

FetchContent_MakeAvailable(SDL3 SDL_image glm)

add_library(GLAD STATIC "glad/src/gl.c")
target_include_directories(GLAD PUBLIC "glad/include")