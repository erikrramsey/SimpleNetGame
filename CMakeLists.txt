# CMakeList.txt : CMake project for SimpleNetGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project("SimpleNetGame")
set(CMAKE_BUILD_TYPE "Debug")

# Add source to this project's executable.
add_executable(SimpleNetGame "src/main.cpp"  "src/Application.cpp"  "src/RendererOpenGL.cpp"  "src/NetworkSessionGGPO.cpp" "src/SessionState.h" "src/GameplayManager.h" "src/GameplayManager.cpp" "src/Input.h" "src/Input.cpp" "src/GameObject.h" "src/BoundingBox.h")

set(vend ${CMAKE_SOURCE_DIR}/vendor)
target_include_directories(SimpleNetGame PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(SimpleNetGame PUBLIC ${vend}/GGPO/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/GLFW/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/glew-2.1.0/include)
target_include_directories(SimpleNetGame PUBLIC ${vend}/glm)

set(ggpo_lib ${vend}/GGPO/lib/GGPO.lib)
set(glfw_lib ${vend}/GLFW/lib/glfw3.lib)
set(glew_lib ${vend}/glew-2.1.0/lib/Release/x64/glew32s.lib)
set(libs ${ggpo_lib} ${glfw_lib} ${glew_lib} winmm.lib ws2_32.lib opengl32)

target_link_libraries(SimpleNetGame ${libs})
