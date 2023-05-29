#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

const char* const vertex_shader = R"(
#version 330 core

layout (location = 0) in vec4 aPos;

out vec2 TexCoords;

uniform mat4 transform;
uniform mat4 projection;

void main() {
    TexCoords = aPos.zw;
	gl_Position = projection * transform *  vec4(aPos.xy, 0.0f, 1.0f);
}
)";

const char* const frag_shader = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main() {
    color = texture(image, TexCoords);
}
)";
