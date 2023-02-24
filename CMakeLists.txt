# CMakeList.txt : CMake project for SimpleNetGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project("SimpleNetGame")
set(CMAKE_BUILD_TYPE "Debug")

set(vend ${CMAKE_SOURCE_DIR}/vendor)
set (IMGUI_DIR ${vend}/imgui)

set(IMGUI_SOURCES
    # Main Imgui files
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"

    # SDL2+OpenGL-specific files
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

file(GLOB_RECURSE main_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB main_includes ${CMAKE_SOURCE_DIR}/src/*)
message("${main_includes}")


# Add source to this project's executable.
add_executable(SimpleNetGame ${main_sources} ${IMGUI_SOURCES})
target_include_directories(SimpleNetGame PUBLIC ${main_includes})
target_include_directories(SimpleNetGame PUBLIC ${vend}/GGPO/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/GLFW/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/glew-2.1.0/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/glm)
target_include_directories(SimpleNetGame PUBLIC ${vend}/steam/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/imgui)


set(ggpo_lib ${vend}/GGPO/lib/GGPO.lib)
set(glfw_lib ${vend}/GLFW/lib/glfw3.lib)
set(glew_lib ${vend}/glew-2.1.0/lib/Release/x64/glew32s.lib)
set(steam_lib ${vend}/steam/lib/steam_api.lib)
set(steam_lib ${vend}/steam/lib/steam_api64.lib)
set(libs ${ggpo_lib} ${glfw_lib} ${glew_lib} ${steam_lib} winmm.lib ws2_32.lib opengl32)

target_link_libraries(SimpleNetGame ${libs})
