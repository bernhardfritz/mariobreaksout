#include "enemy.hpp"
#include "defines.hpp"

void Enemy::setup(piksel::Spritesheet& spritesheet) {
    startPos = center;

    animations[WALKING].frames.push_back(spritesheet.sprites[35]);
    animations[WALKING].frames.push_back(spritesheet.sprites[36]);

    animations[DEAD].frames.push_back(spritesheet.sprites[37]);
}

void Enemy::update(int width, Paddle& paddle, std::vector<Brick>& bricks, Character& character, int currentTimeInMillis, bool winOrLose) {
    if (state == DEAD) {
        return;
    }
    glm::vec2 acceleration = (center.x > BRICK_WIDTH && center.x < (width - BRICK_WIDTH)) ? GRAVITY : glm::vec2(0.0f, 0.0f);
    velocity += acceleration;
    if (velocity.x < -ENEMY_MAX_VELOCITY.x) {
        velocity.x = -ENEMY_MAX_VELOCITY.x;
    } else if (velocity.x > ENEMY_MAX_VELOCITY.x) {
        velocity.x = ENEMY_MAX_VELOCITY.x;
    }
    if (velocity.y < -ENEMY_MAX_VELOCITY.y) {
        velocity.y = -ENEMY_MAX_VELOCITY.y;
    } else if (velocity.y > ENEMY_MAX_VELOCITY.y) {
        velocity.y = ENEMY_MAX_VELOCITY.y;
    }
    center += velocity;
    if (getTop() > startPos.y) {
        if (getLeft() < BRICK_WIDTH) {
            center.x = BRICK_WIDTH + halfDimension.x + 1;
            velocity.x *= -1.0f;
        } else if (getRight() > width - BRICK_WIDTH) {
            center.x = width - BRICK_WIDTH - halfDimension.x - 1;
            velocity.x *= -1.0f;
        }
    }
    for (Brick& brick : bricks) {
        if (intersectsAABB(brick)) {
            if (getBottom() > brick.center.y) {
                if (velocity.x > 0.0f && getRight() > brick.getLeft()) {
                    center.x = brick.getLeft() - halfDimension.x - 1;
                    velocity.x *= -1.0f;
                } else if (velocity.x < 0.0f && getLeft() < brick.getRight()) {
                    center.x = brick.getRight() + halfDimension.x + 1;
                    velocity.x *= -1.0f;
                }
            } else {
                center.y = brick.center.y - brick.halfDimension.y - halfDimension.y;
                velocity.y = 0.0f;
            }
        }
    }
    if (!winOrLose) {
        if (intersectsAABB(character)) {
            if (character.getVelocity().y > 0.0 && character.getBottom() < center.y) {
                state = DEAD;
                timeOfDeathInMillis = currentTimeInMillis;
                velocity.x = 0.0f;
                character.bounce();
            } else if (!character.isInvincible()) {
                character.dieByEnemy(currentTimeInMillis, character.center.x < center.x ? -1.0f : 1.0f);
            }
        }
    }
}

bool Enemy::isOutOfBounds(int left, int right, int top, int bottom) {
    return center.x < left || center.x > right || center.y < top || center.y > bottom;
}

void Enemy::draw(piksel::Graphics& g) {
    g.push();
    g.imageMode(piksel::DrawMode::RADIUS);
    animations[state].draw(g, center.x, center.y, halfDimension.x, halfDimension.y);
    g.pop();
}

int Enemy::getTimeOfDeathInMillis() {
    return timeOfDeathInMillis;
}