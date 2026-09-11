# vendor/CMakeList.txt

include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Enable FetchContent Logging")

FetchContent_Declare(
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG release-3.4.16
		GIT_SHALLOW ON
		GIT_PROGRESS ON
)

FetchContent_MakeAvailable(SDL3)