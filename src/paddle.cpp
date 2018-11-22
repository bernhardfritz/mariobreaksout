#include "paddle.hpp"

bool Paddle::intersectsAABB(AABB& other) {
    return fabs(center.y - other.center.y) <= halfDimension.y + other.halfDimension.y && getLeft() <= other.center.x && other.center.x <= getRight();
}

void Paddle::update(int left, int right, glm::vec2 mousePos) {
    prevCenter = center;
    center.x = mousePos.x;
    if (center.x - halfDimension.x < left) {
        center.x = left + halfDimension.x;
    } else if (center.x + halfDimension.x > right) {
        center.x = right - halfDimension.x;
    }
    deltas[deltaIndex] = center.x - prevCenter.x;
    deltaIndex = (deltaIndex + 1) % 10;
}

void Paddle::draw(piksel::Graphics& g) {
    g.push();
    g.rectMode(piksel::DrawMode::RADIUS);
    g.fill(color);
    g.noStroke();
    g.rect(center.x, center.y, halfDimension.x, halfDimension.y);
    g.pop();
}

float Paddle::getAvgDelta() {
    float sum;
    for (int i = 0; i < 10; i++) {
        sum += deltas[i];
    }
    return sum / 10.0f;
}