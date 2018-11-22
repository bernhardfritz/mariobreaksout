#ifndef BRICK_HPP
#define BRICK_HPP

#include "aabb.hpp"
#include <piksel/graphics.hpp>

class Brick : public AABB {
public:
    Brick(glm::vec2 center, glm::vec2 halfDimension, glm::vec4 color)
        : AABB(center, halfDimension)
        , color(color) {
    }
    void draw(piksel::Graphics& g);
    glm::vec4 getColor();
private:
    glm::vec4 color;
};

#endif /* BRICK_HPP */
