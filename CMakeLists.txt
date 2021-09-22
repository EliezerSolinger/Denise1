
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_STANDARD 99)

SET(BUILD_SHARED_LIBS FALSE)

project(Denise1)

find_package(OpenGL REQUIRED)
add_subdirectory("dependencies")

include_directories(
	${OPENGL_INCLUDE_DIRS} 
	${PROJECT_SOURCE_DIR}
	"dependencies"
	"dependencies/glad/include"
	"dependencies/glfw-3.3.2/include"
)

add_executable(denise1
	src/core/dmath.c
	src/main.c
)
target_link_libraries(denise1 stb_image glad glfw)


