#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "aabb.hpp"
#include <piksel/graphics.hpp>

class Paddle : public AABB {
public:
    Paddle(glm::vec2 center, glm::vec2 halfDimension, glm::vec4 color)
        : AABB(center, halfDimension)
        , color(color)
        , prevCenter(center)
        , deltaIndex(0) {
    }
    bool intersectsAABB(AABB& other);
    void update(int left, int right, glm::vec2 mousePos);
    void draw(piksel::Graphics& g);
    float getAvgDelta();
private:
    glm::vec4 color;
    glm::vec2 prevCenter;
    int deltaIndex;
    float deltas[10] = { 0.0f };
};

#endif /* PADDLE_HPP */