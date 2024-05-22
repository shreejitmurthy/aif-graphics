#pragma once


#include "../vendor/glad/glad.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>

class Math {
public:
    // Using GLM's vec2 and vec3 types
    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;

    // Convert 3D vertex to 2D screen space
    static Vec2 ConvertToScreenSpace(const Vec3& vertex, int screenWidth, int screenHeight) {
        // Projection and normalization are simplified using GLM operations
        Vec2 screenSpaceVertex = Vec2((vertex.x + 1.0f) * 0.5f * screenWidth,
                                      (1.0f - vertex.y) * 0.5f * screenHeight);
        return screenSpaceVertex;
    }

    static Vec3 ConvertTo3DSpace(const Vec2& screenVertex, float z, int screenWidth, int screenHeight) {
        // Reversing screen space normalization
        Vec3 normalizedVertex = Vec3((2.0f * screenVertex.x / screenWidth) - 1.0f,
                                     1.0f - (2.0f * screenVertex.y / screenHeight),
                                     z);
        return normalizedVertex;
    }

    static std::vector<Vec2> ConvertVerticesToScreenSpace(const std::vector<Vec3>& vertices, int screenWidth, int screenHeight) {
        std::vector<Vec2> screenSpaceVertices;
        screenSpaceVertices.reserve(vertices.size());

        for (const auto& vertex : vertices) {
            screenSpaceVertices.push_back(ConvertToScreenSpace(vertex, screenWidth, screenHeight));
        }

        return screenSpaceVertices;
    }

    static std::vector<Vec3> ConvertScreenSpaceToVertices(const std::vector<Vec2>& screenSpaceVertices, float z, int screenWidth, int screenHeight) {
        std::vector<Vec3> vertices;
        vertices.reserve(screenSpaceVertices.size());

        for (const auto& screenVertex : screenSpaceVertices) {
            vertices.push_back(ConvertTo3DSpace(screenVertex, z, screenWidth, screenHeight));
        }

        return vertices;
    }
};

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
    Texture loadImage(const char* path, TextureFilter filter = {FilterParams::LINEAR, FilterParams::LINEAR});
    void drawImage(Texture texture);
    void drawImage(Texture texture, float x, float y);
private:
    unsigned int VBO, VAO, EBO;
    S_Shader textureShader;
};

void setVPT(int width, int height);