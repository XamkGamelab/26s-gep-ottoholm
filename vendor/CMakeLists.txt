# vendor/CMakeList.txt

include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Enable FetchContent logging")

# SDL 3 githubista
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.4.16 
	GIT_SHALLOW ON
	GIT_PROGRESS ON
)

# SDL image githubista
FetchContent_Declare(
	SDL3_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG release-3.4.6 
	GIT_SHALLOW ON
	GIT_PROGRESS ON
	GIT_SUBMODULES ""   # don't clone external/ (aom, dav1d, libjxl, ...)
)

# Use only SDL_image's built-in decoders (stb_image handles PNG and JPG)
set(SDLIMAGE_VENDORED   OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PNG_LIBPNG OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF       OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF        OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP       OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL        OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(SDL3_image)