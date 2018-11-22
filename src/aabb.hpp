#ifndef AABB_HPP
#define AABB_HPP

#include <piksel/graphics.hpp>

class AABB {
public:
    glm::vec2 center;
    glm::vec2 halfDimension;

    AABB(glm::vec2 center, glm::vec2 halfDimension)
        : center(center)
        , halfDimension(halfDimension) {
    };

    bool intersectsAABB(AABB& other);
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
};

#endif /* AABB_HPP */
