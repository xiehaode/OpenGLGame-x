//
// Created by 谢豪 on 2025/8/20.
//

#include "collisionDetector.h"
#include <algorithm> // 用于std::clamp

float collisionDetector::clamp_float(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

// 使用碰撞箱中心和半长检测碰撞
bool collisionDetector::checkCollision(
    const glm::vec3& bulletPos,
    float bulletRadius,
    const glm::vec3& boxPos,
    const glm::vec2& boxHalfExtents
) {
    // 计算碰撞箱在XZ轴的最小和最大边界
    float boxMinX = boxPos.x - boxHalfExtents.x;
    float boxMaxX = boxPos.x + boxHalfExtents.x;
    float boxMinZ = boxPos.z - boxHalfExtents.y;
    float boxMaxZ = boxPos.z + boxHalfExtents.y;

    // 调用重载版本进行检测
    return checkCollision(bulletPos, bulletRadius, boxMinX, boxMaxX, boxMinZ, boxMaxZ);
}

// 使用最小/最大边界检测碰撞
bool collisionDetector::checkCollision(
    const glm::vec3& bulletPos,
    float bulletRadius,
    float boxMinX, float boxMaxX,
    float boxMinZ, float boxMaxZ
) {
    // 将子弹位置投影到碰撞箱的XZ边界上
    // 找到子弹在碰撞箱上的最近点
    float closestX = clamp_float(bulletPos.x, boxMinX, boxMaxX);
    float closestZ = clamp_float(bulletPos.z, boxMinZ, boxMaxZ);

    // 计算子弹到最近点的距离（仅XZ轴）
    float distanceX = bulletPos.x - closestX;
    float distanceZ = bulletPos.z - closestZ;
    float distanceSquared = distanceX * distanceX + distanceZ * distanceZ;

    // 如果距离的平方小于子弹半径的平方，则发生碰撞
    return distanceSquared <= bulletRadius * bulletRadius;
}