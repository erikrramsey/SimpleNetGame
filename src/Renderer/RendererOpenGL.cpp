#include "RendererOpenGL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sng {


RendererOpenGL::RendererOpenGL(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glewInit();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag_shader, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    transLoc = glGetUniformLocation(shaderProgram, "transform");
    projLoc = glGetUniformLocation(shaderProgram, "projection");

	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
}

void RendererOpenGL::begin() {
    const glm::vec2 resolution = { 1920.0f, 1080.0f };
    glUseProgram(shaderProgram);
    glClear(GL_COLOR_BUFFER_BIT);
    glm::mat4 ortho = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(ortho));
}

void RendererOpenGL::drawQuad(glm::vec2& pos) {
    auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    glUniformMatrix4fv(transLoc, 1, false, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RendererOpenGL::loadSprite(const std::string &filename, Sprite &sprite) {
    int n = 0;

    GLuint texture;
    glGenTextures(1, &texture);
    sprite.texture = texture;
    glBindTexture(GL_TEXTURE_2D, sprite.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(filename.c_str(), &sprite.width, &sprite.height, &n, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sprite.width, sprite.height, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void RendererOpenGL::drawSprite(Transform& trans, Sprite& sprite) {
    auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(trans.position, 0.0f));
    transform = glm::scale(transform, glm::vec3(sprite.width, sprite.height, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.texture);

    glUniformMatrix4fv(transLoc, 1, false, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void RendererOpenGL::end() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
    //glUseProgram(0);
    //glBindVertexArray(0);
}

void RendererOpenGL::setClearColor(const glm::vec4 &color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void RendererOpenGL::resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

}
