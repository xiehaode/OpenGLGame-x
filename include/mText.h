#ifndef MTEXT_H
#define MTEXT_H

#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

// 字符结构体，存储单个字形的信息
struct Character {
    unsigned int TextureID; // 字形纹理ID
    glm::ivec2   Size;      // 字形大小
    glm::ivec2   Bearing;   // 从基准线到字形左/顶部的偏移
    unsigned int Advance;   // 到下一个字形的水平偏移
};

class mText {
public:
    mText(const std::string& fontPath = "resources/fonts/Antonio-Bold.ttf");
    ~mText();

    // 渲染文本
    void draw(const std::string& text, float x, float y, float scale,
              const glm::vec3& color, glm::mat4 projection);

private:
    // 初始化缓冲区
    void initBuffers();
    // 加载字体
    bool loadFont(const std::string& fontPath);

    Shader* textShader;               // 文本渲染着色器
    std::map<GLchar, Character> chars;// 存储字符信息的映射表
    unsigned int VAO, VBO;            // 顶点数组和缓冲对象
};

#endif // MTEXT_H

