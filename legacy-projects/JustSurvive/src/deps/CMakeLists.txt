add_subdirectory("glfw-3.3.2")

add_library(glad STATIC
		glad/glad.c
		glad/glad.h
)

add_library(stb_image STATIC
		stb_image/stb_image.c
		stb_image/stb_image.h
)