#include "character.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include <piksel/ext/rng.hpp>

void Character::setup(piksel::Spritesheet& spritesheet, int height) {
    int millisPerFrame = 60;
    animations[IDLE].frames.push_back(spritesheet.sprites[0]);
    animations[IDLE].millisPerFrame = millisPerFrame;

    animations[RUNNING].frames.push_back(spritesheet.sprites[3]);
    animations[RUNNING].frames.push_back(spritesheet.sprites[1]);
    animations[RUNNING].frames.push_back(spritesheet.sprites[2]);
    animations[RUNNING].millisPerFrame = millisPerFrame;

    animations[SLIDING].frames.push_back(spritesheet.sprites[4]);
    animations[SLIDING].millisPerFrame = millisPerFrame;

    animations[JUMPING].frames.push_back(spritesheet.sprites[5]);
    animations[JUMPING].millisPerFrame = millisPerFrame;

    animations[DUCKING].frames.push_back(spritesheet.sprites[6]);
    animations[DUCKING].millisPerFrame = millisPerFrame;

    animations[DYING].frames.push_back(spritesheet.sprites[27]);
    animations[DYING].millisPerFrame = millisPerFrame;

    bluePortal.setup(spritesheet, height);
    orangePortal.setup(spritesheet, height);
}

void Character::update(int left, int right, int top, int bottom, Paddle& paddle, std::vector<Brick>& bricks, int currentTimeInMillis, int mid, bool winOrLose) {
    if (currentTimeInMillis - timeOfDeathInMillis > 3000) {
        invincible = false;
    }
    glm::vec2 acceleration = runAcceleration + jumpAcceleration;
    if (!grounded) {
        acceleration += GRAVITY;
    }
    if (runAcceleration.x == 0.0f) {
        velocity.x *= 0.8f;
    }
    velocity += acceleration;
    jumpAcceleration *= 0.8f;
    if (velocity.x < -CHARACTER_MAX_VELOCITY.x) {
        velocity.x = -CHARACTER_MAX_VELOCITY.x;
    } else if (velocity.x > CHARACTER_MAX_VELOCITY.x) {
        velocity.x = CHARACTER_MAX_VELOCITY.x;
    }
    if (velocity.y < -CHARACTER_MAX_VELOCITY.y) {
        velocity.y = -CHARACTER_MAX_VELOCITY.y;
    } else if (velocity.y > CHARACTER_MAX_VELOCITY.y) {
        velocity.y = CHARACTER_MAX_VELOCITY.y;
    }
    center += velocity;
    if (getLeft() < left) {
        center.x = left + halfDimension.x;
        velocity.x = 0.0f;
    } else if (getRight() > right) {
        center.x = right - halfDimension.x;
        velocity.x = 0.0f;
    } else if (getTop() < top) {
        center.y = top + halfDimension.y;
    } else if (getBottom() > bottom) {
        // lose
    }
    grounded = false;
    for (Brick& brick : bricks) {
        if (intersectsAABB(brick)) {
            if (getBottom() > brick.center.y) {
                if (getTop() > brick.center.y) {
                    if (velocity.y < 0.0f) {
                        center.y = brick.getBottom() + halfDimension.y + 1;
                        velocity.y = 0.0f;
                    }
                } else {
                    if (velocity.x > 0.0f && getRight() > brick.getLeft()) {
                        center.x = brick.getLeft() - halfDimension.x - 1;
                        velocity.x = 0.0f;
                    } else if (velocity.x < 0.0f && getLeft() < brick.getRight()) {
                        center.x = brick.getRight() + halfDimension.x + 1;
                        velocity.x = 0.0f;
                    }
                }
            } else {
                grounded = true;
                center.y = brick.center.y - brick.halfDimension.y - halfDimension.y;
                velocity.y = 0.0f;
            }
        }
    }

    if (!winOrLose) {
        if (center.y > bottom - 64) {
            orangePortal.open(currentTimeInMillis, center.x + (64 / CHARACTER_MAX_VELOCITY.y)  * velocity.x);

            if (bluePortal.getState() == CLOSED) {
                std::vector<Brick> topmostBricks;
                for (Brick& brick : bricks) {
                    if (topmostBricks.empty() || brick.center.y <= topmostBricks.back().center.y) {
                        topmostBricks.push_back(brick);
                    } else {
                        break;
                    }
                }
                bluePortal.open(currentTimeInMillis, topmostBricks.empty() ? mid : topmostBricks[int(piksel::Rng::getInstance().random(0, topmostBricks.size()))].center.x);
            }
        }
    }
    orangePortal.update(currentTimeInMillis);
    bluePortal.update(currentTimeInMillis);

    if (center.y > bottom) {
        if (!winOrLose) {
            dieByOutOfBounds(currentTimeInMillis, top);
        }
        return;
    }
    if (acceleration.y < 0.0f && !grounded) {
        state = JUMPING;
    } else {
        if (grounded) {
            if (fabs(velocity.x) < 0.05f) {
                state = IDLE;
            } else {
                if ((velocity.x < 0.0f && runAcceleration.x > 0.0f) || (velocity.x > 0.0f && runAcceleration.x < 0.0f)) {
                    state = SLIDING;
                } else {
                    state = RUNNING;
                }
            }
        }
    }
    if (lives == 0) {
        state = DYING;
    }
}

void Character::draw(piksel::Graphics& g) {
    orangePortal.draw(g);
    bluePortal.draw(g);
    if (invincible && (g.millis() / 100) % 2 == 0) {
        return;
    }
    g.push();
    g.imageMode(piksel::DrawMode::RADIUS);
    animations[state].millisPerFrame = int(utils_map(fabs(velocity.x), 0.0f, CHARACTER_MAX_VELOCITY.x, CHARACTER_ANIMATION_MAX_MILLIS_PER_FRAME, CHARACTER_ANIMATION_MIN_MILLIS_PER_FRAME));
    animations[state].draw(g, center.x, center.y, state == DYING ? 12 : halfDimension.x, state == DYING ? 12 : halfDimension.y, towardsLeft);
    g.pop();
}

void Character::startRunning(bool towardsLeft) {
    this->towardsLeft = towardsLeft;
    runAcceleration = towardsLeft ? -CHARACTER_RUN_ACCELERATION : CHARACTER_RUN_ACCELERATION;
}

void Character::stopRunning(bool towardsLeft) {
    if (this->towardsLeft == towardsLeft) {
        runAcceleration = glm::vec2(0.0f, 0.0f);
    }
}

void Character::startJumping() {
    if (!grounded) {
        return;
    }
    jumpAcceleration = CHARACTER_JUMP_ACCELERATION;
}

void Character::stopJumping() {
    jumpAcceleration = glm::vec2(0.0f, 0.0f);
}

void Character::startDucking() {
}

void Character::stopDucking() {
}

glm::vec2 Character::getVelocity() {
    return velocity;
}

void Character::bounce() {
    if (velocity.y > 0.0f) {
        velocity.y *= -1.0f;
    }
    invincible = true; // let character be invincible for one frame
}

void Character::dieByEnemy(int currentTimeInMillis, float repellDirection) {
    if (lives > 0) {
        lives--;
    }
    timeOfDeathInMillis = currentTimeInMillis;
    velocity.x = repellDirection * 5.0f;
    if (lives > 0) {
        invincible = true;
    }
}

void Character::dieByOutOfBounds(int currentTimeInMillis, int top) {
    if (currentTimeInMillis - timeOfDeathInMillis > 3000) {
        if (lives > 0) {
            lives--;
        }
        timeOfDeathInMillis = currentTimeInMillis;
        invincible = true;
    }
    center.x = bluePortal.center.x;
    center.y = top + CHARACTER_HEIGHT / 2.0f;
}

bool Character::isInvincible() {
    return invincible;
}

int Character::getLives() {
    return lives;
}

void Character::reset(int width, bool resetCenterX) {
    center = glm::vec2(resetCenterX ? width / 2 : center.x, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - CHARACTER_HEIGHT / 2.0f), glm::vec2(CHARACTER_WIDTH / 2.0f, CHARACTER_HEIGHT / 2.0f);
    lives = 3;
}