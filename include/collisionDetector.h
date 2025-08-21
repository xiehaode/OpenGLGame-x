//
// Created by 谢豪 on 2025/8/20.
//

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H


#include <glm/glm.hpp>

// 碰撞检测类，专注于X和Z轴的碰撞
class collisionDetector {
public:
    // 检测子弹与碰撞箱是否在XZ平面上碰撞
    // 参数：
    // - bulletPos: 子弹在世界空间中的位置
    // - bulletRadius: 子弹碰撞半径（球体）
    // - boxPos: 碰撞箱中心位置
    // - boxHalfExtents: 碰撞箱半长（X和Z方向的一半大小）
    static bool checkCollision(
        const glm::vec3& bulletPos,
        float bulletRadius,
        const glm::vec3& boxPos,
        const glm::vec2& boxHalfExtents
    );

    // 重载版本：使用最小/最大边界定义的碰撞箱
    static bool checkCollision(
        const glm::vec3& bulletPos,
        float bulletRadius,
        float boxMinX, float boxMaxX,  // X轴范围
        float boxMinZ, float boxMaxZ   // Z轴范围
    );
    static float clamp_float(float value, float min, float max);
};




#endif //COLLISIONDETECTOR_H
