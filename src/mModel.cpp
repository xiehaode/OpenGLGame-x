//
// Created by 谢豪 on 2025/8/20.
//

#include "mModel.h"

mModel::mModel() {
    modelShader = new Shader("shader/model_loading.vert", "shader/model_loading.frag");
    ourModel = new Model("resources/objects/nanosuit/nanosuit.obj");
}

void mModel::draw(glm::mat4 view, glm::mat4 projection) {
    modelShader->use();
    //create model
    glm::mat4 model= glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(p.x, -0.5f, p.z));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1));

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setMat4("model", model);
    ourModel->Draw(*modelShader);
}

void mModel::setP(float x1, float z1) {
    p.x=x1;
    p.z=z1;
}



void mModel::initBuff() {

}

unsigned int mModel::loadTexture(char const *path) {

}
