//
// Created by minec on 3/16/2026.
//

#ifndef OOP_SHADERSTATICS_H
#define OOP_SHADERSTATICS_H

#include <fstream>
#include <iostream>
#include <map>
#include "../libs/glad/include/glad/glad.h" // 1. GLAD FIRST
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace ShaderStatics {

    static std::string loadShaderSource(std::string filename) {
        std::ifstream file("shaders/" + filename);
        if (!file.is_open()) {
            std::cout << "CRITICAL ERROR: Could not find or open shader file: " << filename << std::endl;
            return ""; // Return empty to prevent a crash, but the console will now yell at you.
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static unsigned int createShaderProgram(std::string vertPath, std::string fragPath) {
        std::string vertSrc = loadShaderSource(vertPath);
        std::string fragSrc = loadShaderSource(fragPath);
        const char* vCode = vertSrc.c_str();
        const char* fCode = fragSrc.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCode, NULL);
        glCompileShader(vertex);

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCode, NULL);
        glCompileShader(fragment);

        unsigned int ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return ID;
    }
}

#endif //OOP_SHADERSTATICS_H