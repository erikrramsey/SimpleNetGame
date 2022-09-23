#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

const char* const vertex_shader = R"(
#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 transform;

void main() {
	gl_Position = transform *  vec4(aPos, 1.0f, 1.0f);
}
)";

const char* const frag_shader = R"(
#version 330 core
out vec4 FragColor;

void main() {
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";
