//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MMODEL_H
#define MMODEL_H
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

class mModel {
public:
    mModel();
    void draw(glm::mat4 view,glm::mat4 projection);
private:
    void initBuff();
    unsigned int loadTexture(char const * path);
    Model* ourModel;

    Shader *modelShader;
    // plane VAO
    unsigned int modelVAO, modelVBO;
};


#endif //MMODEL_H
