#include "graphics.hpp"

Graphics::Graphics() {
}

Graphics::~Graphics() {
}

void Graphics::clearBackground(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}
