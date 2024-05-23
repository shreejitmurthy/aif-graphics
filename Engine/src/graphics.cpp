#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "graphics.hpp"
#include "math.hpp"

void setVPT(int w, int h) {
    glViewport(0, 0, w, h);
}

void Graphics::Texture::scale(float sx, float sy) {
    this->sx = sx;
    this->sy = sy;
}

void Graphics::Texture::set_position(float x, float y) {
    this->px = x;
    this->py = y;
    this->pos_set = true;
}

void Graphics::Texture::rotate(float r) {
    this->r = r;
}

// load glad + setup vertex data (and buffer(s)) and configure vertex attributess
Graphics::Graphics(GLADloadproc proc, int w, int h) : screenWidth(w), screenHeight(h) {
    if (!gladLoadGLLoader(proc)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }
    stbi_set_flip_vertically_on_load(true);
    textureShader = new_shader("Engine/shaders/texture.vs", "Engine/shaders/texture.fs");
    setupBuffers();
}

Graphics::~Graphics() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Graphics::setupBuffers() {
        // float
    static constexpr auto vertices = std::array{
        // positions          // colours          // texture coords
        1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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

Graphics::Texture Graphics::loadImage(const char* path, TextureFilter filter) {
    Texture texture;
    glGenTextures(1, &texture.ID);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter.min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter.mag);
    texture.data = stbi_load(path, &texture.width, &texture.height, &texture.nrChannels, 4);
    if (texture.data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture.data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    return texture;
}

void Graphics::clearBackground(Colour color) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::drawImage(Texture& texture, float x, float y) {
    texture.transform = glm::mat4(1.0f);

    texture.set_position(x, y);
    texture.transform = glm::translate(texture.transform, Math::ConvertTo3DSpace(glm::vec2(texture.px, texture.py), 1.f, screenWidth, screenHeight));

    // check if scale has been set and scale appropriately.
    if (texture.sx == 1.f && texture.sy == 1.f) {
        float scaleX = static_cast<float>(texture.width) / screenWidth;
        float scaleY = static_cast<float>(texture.height) / screenHeight;
        texture.transform = glm::scale(texture.transform, glm::vec3(scaleX, scaleY, 1.f));
    } else {
         texture.transform = glm::scale(texture.transform, glm::vec3(texture.sx, texture.sy, 1.f));
    }

    if (texture.r >= 0) {
        texture.transform = glm::rotate(texture.transform, texture.r, glm::vec3(texture.ox, texture.oy, 1.0f));
    }

    // Apply the transformations and draw
    bind_and_draw(texture);
}

void Graphics::bind_and_draw(const Texture& texture) {
    use_shader(textureShader);
    glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(texture.transform));
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

