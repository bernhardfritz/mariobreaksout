#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "aabb.hpp"
#include "brick.hpp"
#include "paddle.hpp"
#include "portal.hpp"
#include <piksel/ext/animation.hpp>
#include <piksel/ext/spritesheet.hpp>

enum CharacterState {
    IDLE = 0,
    RUNNING,
    SLIDING,
    JUMPING,
    DUCKING,
    DYING
};

class Character : public AABB {
public:
    Character(glm::vec2 center, glm::vec2 halfDimension)
        : AABB(center, halfDimension)
        , state(IDLE)
        , towardsLeft(false)
        , grounded(false)
        , velocity(0.0f, 0.0f)
        , runAcceleration(0.0f, 0.0f)
        , jumpAcceleration(0.0f, 0.0f)
        , invincible(false)
        , timeOfDeathInMillis(-1)
        , lives(3)
        , bluePortal(true)
        , orangePortal(false)
    {}
    void setup(piksel::Spritesheet& spritesheet, int height);
    void update(int left, int right, int top, int bottom, Paddle& paddle, std::vector<Brick>& bricks, int currentTimeInMillis, int mid, bool winOrLose);
    void draw(piksel::Graphics& g);
    void startRunning(bool towardsLeft);
    void stopRunning(bool towardsLeft);
    void startJumping();
    void stopJumping();
    void startDucking();
    void stopDucking();
    glm::vec2 getVelocity();
    void bounce();
    void dieByEnemy(int currentTimeInMillis, float repellDirection);
    void dieByOutOfBounds(int currentTimeInMillis, int top);
    bool isInvincible();
    int getLives();
    void reset(int width, bool resetCenterX);
private:
    piksel::Animation animations[6];
    CharacterState state;
    bool towardsLeft;
    bool grounded;
    glm::vec2 velocity;
    glm::vec2 runAcceleration;
    glm::vec2 jumpAcceleration;
    bool invincible;
    int timeOfDeathInMillis;
    int lives;
    Portal bluePortal;
    Portal orangePortal;
};

#endif /* CHARACTER_HPP */