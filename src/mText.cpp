#include "mText.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stb_image.h>
#include <iostream>

// 构造函数，加载字体并初始化
mText::mText(const std::string& fontPath) {
    // 初始化着色器
    textShader = new Shader("shader/text.vs", "shader/text.fs");

    // 初始化缓冲区
    initBuffers();

    // 加载字体
    if (!loadFont(fontPath)) {
        std::cerr << "警告：无法加载字体，文本渲染将不可用" << std::endl;
    }
}

// 析构函数，释放资源
mText::~mText() {
    delete textShader;

    // 删除所有字符纹理
    for (auto& pair : chars) {
        glDeleteTextures(1, &pair.second.TextureID);
    }

    // 删除缓冲对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// 初始化顶点缓冲和数组对象
void mText::initBuffers() {
    // 生成并绑定VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 为6个顶点的四边形预留空间（每个顶点4个float值）
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // 设置顶点属性指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 加载字体文件并创建字符纹理
bool mText::loadFont(const std::string& fontPath) {
    // 清空之前的字符数据
    chars.clear();

    // 初始化FreeType库
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: 无法初始化FreeType库" << std::endl;
        return false;
    }

    // 加载字体 face
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: 无法加载字体文件: " << fontPath << std::endl;
        FT_Done_FreeType(ft);
        return false;
    }

    // 设置字体大小（宽为0表示按高度自动计算）
    FT_Set_Pixel_Sizes(face, 0, 48);

    // 禁用字节对齐限制
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // 加载ASCII字符集中的前128个字符
    for (unsigned char c = 0; c < 128; c++) {
        // 加载字符字形
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: 无法加载字符 '" << c << "'" << std::endl;
            continue;
        }

        // 为当前字符生成纹理
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 存储字符信息
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        chars.insert(std::pair<char, Character>(c, character));
    }

    // 清理FreeType资源
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}

// 渲染文本
void mText::draw(const std::string& text, float x, float y, float scale,
                 const glm::vec3& color, glm::mat4 projection) {
    // 启用混合（用于文本透明度）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 激活着色器并设置参数
    textShader->use();
    textShader->setMat4("projection", projection);
    textShader->setVec3("textColor", color);

    // 激活纹理单元0
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历文本中的每个字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = chars[*c];

        // 计算字符位置
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        // 计算字符大小
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // 更新VBO数据
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        // 绑定当前字符纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // 更新VBO内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // 绘制字符
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 移动到下一个字符的位置
        x += (ch.Advance >> 6) * scale; // 右移6位相当于除以64（将1/64像素单位转换为像素）
    }

    // 清理状态
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

