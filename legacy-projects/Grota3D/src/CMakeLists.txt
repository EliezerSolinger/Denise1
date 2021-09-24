 
)
add_library(core STATIC
	core/array.c
	core/list.c
	core/mathematics.c
	core/colors.c
	core/time.c
)
target_link_libraries(core stb_image glad glfw m lua)

add_library(grota3d SHARED
	graphics/system/window.c
	graphics/system/shaders.c
	graphics/graphics.c
	graphics/ui/ui.c
	graphics/3d/renderer.c
	graphics/3d/camera.c
	graphics/3d/bsp.c
	physics/aabb.c
	modules/obj_loader.c
	modules/scripting.c
	modules/heightmap.c
)
target_link_libraries(grota3d core)

add_executable(test targets/test.c targets/modelviewer.rc)
target_link_libraries(test grota3d)

add_executable(modelviewer targets/modelviewer.c targets/modelviewer.rc)
target_link_libraries(modelviewer grota3d)

add_executable(player targets/player.c targets/modelviewer.rc)
target_link_libraries(player grota3d )

add_executable(phx targets/phx.c)
target_link_libraries(phx grota3d )

