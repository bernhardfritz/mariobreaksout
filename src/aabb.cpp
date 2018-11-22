#include "aabb.hpp"
#include <piksel/drawmode.hpp>

bool AABB::intersectsAABB(AABB& other) {
    glm::vec2 sum = halfDimension + other.halfDimension;
    return fabs(center.x - other.center.x) <= sum.x
        && fabs(center.y - other.center.y) <= sum.y
    ;
}

float AABB::getLeft() {
    return center.x - halfDimension.x;
}

float AABB::getRight() {
    return center.x + halfDimension.x;
}

float AABB::getTop() {
    return center.y - halfDimension.y;
}

float AABB::getBottom() {
    return center.y + halfDimension.y;
}
