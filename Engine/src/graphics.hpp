#pragma once

#include "../vendor/glad/glad.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

#include "shader.hpp"

#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>

#define WHITE (Colour) {1.0f, 1.0f, 1.0f, 1.0f}
#define BLACK (Colour) {0.0f, 0.0f, 0.0f, 1.0f}
#define TEAL  (Colour) {0.2f, 0.3f, 0.3f, 1.0f}


// this is so useless
enum FilterParams {   
    NEAR   = 0x2600,
    LINEAR = 0x2601
};

struct TextureFilter {
    FilterParams min;
    FilterParams mag;
};

struct Colour {
    float r;
    float g;
    float b;
    float a;
};

class Graphics {
public:

    class Texture {
    public:
        unsigned int ID;
        int width, height, nrChannels;
        unsigned char* data;
        glm::mat4 transform;
        float px, py;
        bool pos_set = false;
        // default values for scale
        float sx = 1.f, sy = 1.f;
        // rotation can never be negative so set to this initially
        float r = -1.f;
        float ox = 0.f, oy = 0.f;

        void set_position(float x, float y);
        void scale(float sx, float sy);
        void rotate(float r);
    };


    Graphics(GLADloadproc proc, int w, int h);
    ~Graphics();

    void clearBackground(Colour col);
    Texture loadImage(const char* path, TextureFilter filter = {FilterParams::LINEAR, FilterParams::LINEAR});

    void drawImage(Texture& texture, float x, float y);
    void drawImage(Texture texture, float x, float y, float sx, float sy, float r, float ox, float oy);
private:
    void bind_and_draw(const Texture& texture);
    void setupBuffers();

    int imageCount;
    unsigned int VBO, VAO, EBO;
    S_Shader textureShader;
    int screenWidth; 
    int screenHeight;
};

void setVPT(int width, int height);