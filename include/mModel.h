//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MMODEL_H
#define MMODEL_H
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
struct position {
    float x;
    float z;
};

class mModel {
public:
    mModel();
    void draw(glm::mat4 view,glm::mat4 projection);
    void setP(float x1,float z1);
private:
    void initBuff();
    unsigned int loadTexture(char const * path);
    Model* ourModel;
    position p;
    Shader *modelShader;
    // plane VAO
    unsigned int modelVAO, modelVBO;
};


#endif //MMODEL_H
