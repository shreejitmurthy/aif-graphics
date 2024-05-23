#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
#include "graphics.hpp"

void scale_image();

void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

S_Shader new_shader(const char* vertexPath, const char* fragmentPath) {
    S_Shader shader;
     // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    checkCompileErrors(shader.ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

void use_shader(S_Shader shader) {
    glUseProgram(shader.ID); 
}

void setVPT(int w, int h) {
    glViewport(0, 0, w, h);
}

void Graphics::Texture::scale(float sx, float sy) {
    this->sx = sx;
    this->sy = sy;
    this->scaled = true;
}

void Graphics::Texture::set_position(float x, float y, int screenWidth, int screenHeight) {
    transform = glm::translate(transform, Math::ConvertTo3DSpace(glm::vec2(x, y), 1.f, screenWidth, screenHeight));
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
    static constexpr float vertices[] = {
        // positions          // colours          // texture coords
        1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    static constexpr unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2); // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
    // Reset the transform to the identity matrix for each frame's drawing
    texture.transform = glm::mat4(1.0f);

    // Set position via the texture's own method
    texture.set_position(x, y, screenWidth, screenHeight);

    // If the texture hasn't been manually scaled, calculate the scaling based on screen size
    if (!texture.scaled) {
        float scaleX = static_cast<float>(texture.width) / screenWidth;
        float scaleY = static_cast<float>(texture.height) / screenHeight;
        texture.transform = glm::scale(texture.transform, glm::vec3(scaleX, scaleY, 1.f));
    } else {
         texture.transform = glm::scale(texture.transform, glm::vec3(texture.sx, texture.sy, 1.f));
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

