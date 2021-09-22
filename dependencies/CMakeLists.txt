
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("glfw-3.3.2")

add_library(glad STATIC
		glad/src/glad.c
)
include_directories(
	"glfw-3.3.2/include"
	"glad/include"
)
add_library(stb_image STATIC
		stb_image/stb_image.c
		stb_image/stb_image.h
)