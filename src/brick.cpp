#include "brick.hpp"
#include "defines.hpp"

void Brick::draw(piksel::Graphics& g) {
    g.push();
    g.rectMode(piksel::DrawMode::RADIUS);
    g.fill(color);
    g.noStroke();
    g.rect(center.x, center.y, halfDimension.x, halfDimension.y);
    g.pop();
}

glm::vec4 Brick::getColor() {
    return color;
}