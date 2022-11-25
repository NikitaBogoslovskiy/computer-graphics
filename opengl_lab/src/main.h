#pragma once
#ifndef __MAIN_H__
#define __MAIN_H__

#include "gl/glew.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#define red 1.0,0.0,0.0,1.0
#define green 0.0,1.0,0.0,1.0
#define blue 0.0,0.0,1.0,1.0
#define yellow 1.0,1.0,0.3,1.0

// �������� ��� ���������� �������
const char* VertexShaderSource = R"(
#version 330 core

in vec2 coord;
in vec4 color;

out vec4 vcolor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(coord, 0.0, 1.0);
    vcolor = color;
    TexCoord = coord + 1.0 * 0.0008;
}
)";

//coord + 1.0 * 0.0008;
//vec4(coord.x, coord.y, 1. - (coord.x + coord.y), 1.0);


const char* FragShaderSource = R"(
#version 330 core

in vec4 vcolor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D img;

void main() {
    FragColor = texture(img, TexCoord) * vcolor;
}
)";

GLuint Program;

GLuint Attrib_vertex;
GLuint Attrib_color;
GLuint Attrib_texture;


GLuint VBO;

GLuint texture0;


struct Vertex {
    GLfloat x;
    GLfloat y;
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

sf::Shader shader;

void checkOpenGLerror() {

}

void ShaderLog(unsigned int shader)
{
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1)
    {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
    }
}

void InitShader() {
    // ������� ��������� ������
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // �������� �������� ���
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    // ����������� ������
    glCompileShader(vShader);
    std::cout << "vertex shader \n";
    // ������� ������ ���� �������
    ShaderLog(vShader);
    // ������� ����������� ������
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // �������� �������� ���
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    // ����������� ������
    glCompileShader(fShader);
    std::cout << "fragment shader \n";
    // ������� ������ ���� �������
    ShaderLog(fShader);
    // ������� ��������� � ����������� ������� � ���
    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    // ������� ��������� ���������
    glLinkProgram(Program);
    // ��������� ������ ������
    int link_ok;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        std::cout << "error attach shaders \n";
        return;
    }
    // ���������� ID �������� �� ��������� ���������
    const char* attr_name = "coord"; //��� � �������
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1) {
        std::cout << "could not bind attrib " << attr_name << std::endl;
        return;
    }
    
    const char* attr_name2 = "color"; //��� � �������
    Attrib_color = glGetAttribLocation(Program, attr_name2);
    if (Attrib_color == -1) {
        std::cout << "could not bind attrib " << attr_name2 << std::endl;
        return;
    }

    checkOpenGLerror();
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int width, height, nrChannels;
unsigned char* data;

void InitVBO() {
    glGenBuffers(1, &VBO);

    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    data = (unsigned char*)stbi_load("images.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    static const float PI = 3.14f;
    static const float DPI = 6.28f;

    Vertex vivi[18] = {
        { -1.0 / 4 - 0.5, 1.0 / 4 + 0.5, red }, // cube
        { 1.0 / 4 - 0.5, 1.0 / 4 + 0.5, green },
        { 1.0 / 4 - 0.5, -1.0 / 4 + 0.5, blue },
        { -1.0 / 4 - 0.5, -1.0 / 4 + 0.5, yellow },
        { -1.0f / 4 + 0.5, -1.0f / 4 + 0.5, red }, // triangle
        { 0.0f / 4 + 0.5, 1.0f / 4 + 0.5, green },
        { 1.0f / 4 + 0.5, -1.0f / 4 + 0.5, blue},
        { 0.0f / 4 - 0.5, -1.0f / 4 - 0.5, red }, // fan
        { -1.0f / 4 - 0.5, 0.0f / 4 - 0.5, green },
        { -0.77f / 4 - 0.5, 0.77f / 4 - 0.5, blue },
        { 0.0f / 4 - 0.5, 1.0f / 4 - 0.5, yellow },
        { 0.77f / 4 - 0.5, 0.77f / 4 - 0.5, red },
        { 1.0f / 4 - 0.5, 0.0f / 4 - 0.5, green },
        { cosf(DPI / 5) / 4 + 0.5, sinf(DPI / 5) / 4 - 0.5, red }, // 5
        { cosf(DPI / 5 * 2) / 4 + 0.5, sinf(DPI / 5 * 2) / 4 - 0.5, green },
        { cosf(DPI / 5 * 3) / 4 + 0.5, sinf(DPI / 5 * 3) / 4 - 0.5, blue },
        { cosf(DPI / 5 * 4) / 4 + 0.5, sinf(DPI / 5 * 4) / 4 - 0.5, yellow },
        { cosf(0.0) / 4 + 0.5, sinf(0.0) / 4 - 0.5, green },
        
    };


    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vivi), vivi, GL_STATIC_DRAW);

    checkOpenGLerror(); //������ ������� ���� � ������������
    // �������� ������ OpenGL, ���� ���� �� ����� � ������� ��� ������
}


void Init() {
    InitShader();
    InitVBO();
}

void Draw() {
    // ������������� ��������� ��������� �������
    glUseProgram(Program);
    // �������� ������ ���������
    glEnableVertexAttribArray(Attrib_vertex);
    glEnableVertexAttribArray(Attrib_color);
    // ���������� VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // �������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(Attrib_vertex);
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(Attrib_color);
    // ��������� VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // �������� ������ �� ����������(������)
    
    //glUniform4f(glGetUniformLocation(Program, "ucol"), (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glUniform1i(glGetUniformLocation(Program, "img"), 0);

    glDrawArrays(GL_QUADS, 0, 4);
    glDrawArrays(GL_TRIANGLES, 4, 3);
    glDrawArrays(GL_TRIANGLE_FAN, 7, 6);
    glDrawArrays(GL_POLYGON, 13, 5);
    
    
    // ��������� ������ ���������
    glDisableVertexAttribArray(Attrib_vertex);
    glDisableVertexAttribArray(Attrib_color);
    // ��������� ��������� ���������
    glUseProgram(0);
    checkOpenGLerror();
}

void ReleaseShader() {
    // ��������� ����, �� ��������� ��������� ���������
    glUseProgram(0);
    // ������� ��������� ���������
    glDeleteProgram(Program);

}

void ReleaseVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
}

void Release() {
    // �������
    ReleaseShader();
    // ��������� �����
    ReleaseVBO();
}


void SetIcon(sf::Window& wnd)
{
    sf::Image image;
    // ������ ��������� ��������, ����� ��������� ������ �� ����� (image.loadFromFile("icon.png"))
    image.create(16, 16);
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            image.setPixel(i, j, { (uint8_t)(i * 16), (uint8_t)(j * 16), 0 });

    wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}

#endif // !__MAIN_H__
