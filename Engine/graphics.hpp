#pragma once
#include "./vendor/glad/glad.h"
#include <iostream>
#include <memory>
#include <array>

#define WHITE (Colour) {1.0f, 1.0f, 1.0f, 1.0f}
#define BLACK (Colour) {0.0f, 0.0f, 0.0f, 1.0f}
#define TEAL  (Colour) {0.2f, 0.3f, 0.3f, 1.0f}

// this is so useless
enum TextureFilter {   
    NEAR   = 0x2600,
    LINEAR = 0x2601
};

struct Colour {
    float r;
    float g;
    float b;
    float a;
};

struct Texture {
    unsigned int ID;
    int width, height, nrChannels;
    unsigned char* data;
};

// static constexpr auto vertices = std::array{
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
// };

// static constexpr auto indices = std::array{
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };

class Graphics {
public:
    Graphics(GLADloadproc proc);
    ~Graphics();

    void clearBackground(Colour col);
    std::unique_ptr<Texture> loadImage(const char* path, TextureFilter mag = TextureFilter::LINEAR);
private:
    unsigned int VBO, VAO, EBO;
};

void setVPT(int width, int height);