#ifndef BALL_HPP
#define BALL_HPP

#include "aabb.hpp"
#include "brick.hpp"
#include "paddle.hpp"
#include <piksel/graphics.hpp>
#include <piksel/ext/timer.hpp>
#include <deque>

class Ball : public AABB {
public:

    Ball(glm::vec2 center, glm::vec2 halfDimension, glm::vec2 velocity)
        : AABB(center, halfDimension)
        , velocity(velocity)
        , lives(3)
        , score(0)
    {}
    void setup(int currentTimeInMillis);
    void update(int left, int right, int top, int bottom, Paddle& paddle, std::vector<Brick>& bricks, int& enemySpawnDelayInMillis, int width, int height, int currentTimeInMillis);
    void draw(piksel::Graphics& g);
    int getLives();
    void die(int width, int height, int currentTimeInMillis);
    int getScore();
    void reset(int width, int height, int currentTimeInMillis);
private:
    glm::vec2 velocity;
    int lives;
    std::deque<glm::vec2> prevCenters;
    int score;
    piksel::Timer respawnTimer;
};

#endif /* BALL_HPP */