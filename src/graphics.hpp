#pragma once
#include "../include/glad/glad.h"

struct Colour {
    float r;
    float g;
    float b;
    float a;
};

class Graphics {
public:
    Graphics();
    ~Graphics();

    void clearBackground(float r, float g, float b, float a);
private:

};