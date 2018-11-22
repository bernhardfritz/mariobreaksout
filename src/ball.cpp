#include "ball.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include <piksel/ext/rng.hpp>
#include <algorithm>

void Ball::setup(int currentTimeInMillis) {
    respawnTimer.reset(currentTimeInMillis);
}

void Ball::update(int left, int right, int top, int bottom, Paddle& paddle, std::vector<Brick>& bricks, int& enemySpawnDelayInMillis, int width, int height, int currentTimeInMillis) {
    if (respawnTimer.getElapsedTimeInMillis(currentTimeInMillis) < 3000) {
        return;
    }
    if ((velocity.x < 0.0f && getLeft() < left) || (velocity.x > 0.0f && getRight() > right)) {
        velocity.x *= -1;
    }
    if (velocity.y < 0.0f && getTop() < top) {
        velocity.y *= -1;
    } else if (velocity.y > 0.0f && center.y > bottom) {
        die(width, height, currentTimeInMillis);
        return;
    }
    if (velocity.y > 0) {
        if (paddle.intersectsAABB(*this)) {
            float delta = paddle.getAvgDelta();
            velocity.x += delta;
            if (velocity.x < -BALL_SPEED) {
                velocity.x = -BALL_SPEED;
            } else if (velocity.x > BALL_SPEED) {
                velocity.x = BALL_SPEED;
            }
            velocity.y *= -1;
        }
    }
    std::vector<Brick>::iterator it = bricks.begin();
    while (it != bricks.end()) {
        Brick& brick = *it;
        if (intersectsAABB(brick)) {
            if ((velocity.x > 0.0f && center.x < brick.getLeft()) || (velocity.x < 0.0f && brick.getRight() < center.x)) {
                velocity.x *= -1.0f;
            }
            if ((velocity.y > 0.0f && center.y < brick.getTop()) || (velocity.y < 0.0f && brick.getBottom() < center.y)) {
                velocity.y *= -1.0f;
            }
            int currentTimeInSeconds = currentTimeInMillis / 1000;
            if (brick.getColor() == RED) {
                score += std::max<int>(1, 150 - currentTimeInSeconds / 10);
            } else if (brick.getColor() == ORANGE) {
                score += std::max<int>(1, 125 - currentTimeInSeconds / 10);
            } else if (brick.getColor() == BROWN) {
                score += std::max<int>(1, 100 - currentTimeInSeconds / 10);
            } else if (brick.getColor() == YELLOW) {
                score += std::max<int>(1, 75 - currentTimeInSeconds / 10);
            } else if (brick.getColor() == GREEN) {
                score += std::max<int>(1, 50 - currentTimeInSeconds / 10);
            } else if (brick.getColor() == BLUE) {
                score += std::max<int>(1, 25 - currentTimeInSeconds / 10);
            }
            it = bricks.erase(it);
            enemySpawnDelayInMillis = utils_map(bricks.size(), 0, 6 * 18, ENEMY_MIN_SPAWN_DELAY_IN_MILLIS, ENEMY_MAX_SPAWN_DELAY_IN_MILLIS);
            break; // only one intersection should be allowed per cycle
        } else {
            it++;
        }
    }
    if (prevCenters.size() > 6) {
        prevCenters.pop_back();
    }
    prevCenters.push_front(center);
    center += velocity;
}

void Ball::draw(piksel::Graphics& g) {
    g.push();
    if (respawnTimer.getElapsedTimeInMillis(g.millis()) < 3000) {
        g.fill(WHITE);
        g.textSize(16);
        g.text(std::to_string(3 - respawnTimer.getElapsedTimeInMillis(g.millis()) / 1000), center.x + 4.0f * velocity.x - 4.0f, center.y + 4.0f * velocity.y + 8.0f);
    }
    g.rectMode(piksel::DrawMode::RADIUS);
    g.noStroke();
    glm::vec4 color = RED;
    g.fill(color);
    g.rect(center.x, center.y, halfDimension.x, halfDimension.y);
    for (int i = 0; i < std::min<int>(prevCenters.size(), 6); i++) {
        color.a -= 0.15f;
        g.fill(color);
        g.rect(prevCenters[i].x, prevCenters[i].y, halfDimension.x, halfDimension.y);
    }
    g.pop();
}

int Ball::getLives() {
    return lives;
}

void Ball::die(int width, int height, int currentTimeInMillis) {
    if (lives > 0) {
        lives--;
    }
    center = glm::vec2(piksel::Rng::getInstance().random(2.0f * BRICK_WIDTH, width - 2.0f * BRICK_WIDTH), height / 2 + 60);
    prevCenters.clear();
    velocity = glm::vec2(piksel::Rng::getInstance().random(-BALL_SPEED, BALL_SPEED), BALL_SPEED);
    respawnTimer.reset(currentTimeInMillis);
}

int Ball::getScore() {
    return score;
}

void Ball::reset(int width, int height, int currentTimeInMillis) {
    lives = 3;
    prevCenters.clear();
    score = 0;
    respawnTimer.reset(currentTimeInMillis);
    center = glm::vec2(piksel::Rng::getInstance().random(2.0f * BRICK_WIDTH, width - 2.0f * BRICK_WIDTH), height / 2 + 60);
    velocity = glm::vec2(piksel::Rng::getInstance().random(-BALL_SPEED, BALL_SPEED), BALL_SPEED);
}