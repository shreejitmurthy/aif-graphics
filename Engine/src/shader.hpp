#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

struct S_Shader {
    unsigned int ID;
};

void checkCompileErrors(unsigned int shader, std::string type);
S_Shader new_shader(const char* vertexPath, const char* fragmentPath);
void use_shader(S_Shader shader);