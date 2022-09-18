# CMakeList.txt : CMake project for SimpleNetGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("SimpleNetGame")

# Add source to this project's executable.
add_executable (SimpleNetGame "main.cpp"  "vendor/ggponet.h" "PlayerInput.h")
set(ggpo ${CMAKE_SOURCE_DIR}/vendor/release/GGPO.lib)
target_link_libraries(SimpleNetGame ${ggpo} winmm.lib ws2_32.lib)

# TODO: Add tests and install targets if needed.
