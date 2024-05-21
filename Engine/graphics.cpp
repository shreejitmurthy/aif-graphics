#include "graphics.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"
#include "vendor/shader_s.h"


void setVPT(int w, int h) {
    glViewport(0, 0, w, h);
}

// load glad + setup vertex data (and buffer(s)) and configure vertex attributess
Graphics::Graphics(GLADloadproc proc) {
    if (!gladLoadGLLoader(proc)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    textureShader = new_shader("shaders/texture.vs", "shaders/texture.fs");

    // float
    static constexpr auto vertices = std::array{
        // positions          // colours          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    // unsigned int
    static constexpr auto indices = std::array{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Graphics::~Graphics() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void Graphics::clearBackground(Colour col) {
    glClearColor(col.r, col.g, col.b, col.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

std::unique_ptr<Texture> Graphics::loadImage(const char* path, TextureFilter mag) {
    std::unique_ptr<Texture> t = std::make_unique<Texture>();

    glGenTextures(1, &t->ID);
    glBindTexture(GL_TEXTURE_2D, t->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

    t->data = stbi_load(path, &t->width, &t->height, &t->nrChannels, 0);
    if (!t->data) {
        std::cerr << "Failed to load texture at: '" << path << "'" << std::endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, t->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(t->data);

    return t;
}