#pragma once
#include "../vendor/glad/glad.h"
#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include <sstream>

struct S_Shader {
    unsigned int ID;
};

void checkCompileErrors(unsigned int shader, std::string type);
S_Shader new_shader(const char* vertexPath, const char* fragmentPath);
void use_shader(S_Shader shader);

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

class Graphics {
public:
    Graphics(GLADloadproc proc);
    ~Graphics();

    void clearBackground(Colour col);
    Texture loadImage(const char* path, TextureFilter mag = TextureFilter::LINEAR);
    void drawImage(Texture texture);
private:
    unsigned int VBO, VAO, EBO;
    S_Shader textureShader;
};

void setVPT(int width, int height);