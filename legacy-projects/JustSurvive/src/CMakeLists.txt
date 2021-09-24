
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_STANDARD 99)

SET(BUILD_SHARED_LIBS FALSE)

SET(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

project(JustSurvive)

find_package(OpenGL REQUIRED)
add_subdirectory("deps")

include_directories(
	${OPENGL_INCLUDE_DIRS} 
	${PROJECT_SOURCE_DIR}
	"deps/glfw-3.3.1/include"
	"deps"
)
add_library(core STATIC
	core/list.c
	core/time.c
)
target_link_libraries(core stb_image glad glfw)

add_library(grota3d SHARED
	graphics/system/window.c
	graphics/system/shaders.c
	graphics/graphics.c
	graphics/ui/ui.c
	graphics/2d/camera2d.c
	graphics/2d/sprites.c
	physics/physics2d.c
	modules/resources.c
)
add_library(physics STATIC 
	physics/physics2d.c
)
target_link_libraries(grota3d core)

add_executable(teste 
	test/test.c
)
target_link_libraries(teste grota3d)


