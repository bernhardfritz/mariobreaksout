#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "aabb.hpp"
#include "brick.hpp"
#include "character.hpp"
#include "paddle.hpp"
#include <piksel/ext/animation.hpp>
#include <piksel/ext/spritesheet.hpp>

enum EnemyState {
    WALKING = 0,
    DEAD
};

class Enemy : public AABB {
public:
    Enemy(glm::vec2 center, glm::vec2 halfDimension, glm::vec2 velocity) : AABB(center, halfDimension), state(WALKING), velocity(velocity), timeOfDeathInMillis(-1) {}
    void setup(piksel::Spritesheet& spritesheet);
    void update(int width, Paddle& paddle, std::vector<Brick>& bricks, Character& character, int currentTimeInMillis, bool winOrLose);
    void draw(piksel::Graphics& g);
    bool isOutOfBounds(int left, int right, int top, int bottom);
    int getTimeOfDeathInMillis();
private:
    piksel::Animation animations[2];
    EnemyState state;
    glm::vec2 velocity;
    glm::vec2 startPos;
    int timeOfDeathInMillis;
};

#endif /* ENEMY_HPP */