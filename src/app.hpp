#ifndef APP_HPP
#define APP_HPP

#include "ball.hpp"
#include "brick.hpp"
#include "defines.hpp"
#include "paddle.hpp"
#include "character.hpp"
#include "enemy.hpp"
#include "firework.hpp"
#include <piksel/baseapp.hpp>
#include <piksel/ext/spritesheet.hpp>
#include <piksel/ext/animation.hpp>
#include <piksel/ext/rng.hpp>
#include <piksel/ext/timer.hpp>

enum AppState {
    STARTSCREEN = 0,
    GAME,
    GAMEOVER,
    VICTORY
};

class App : public piksel::BaseApp {
public:
    App()
        : BaseApp(640, 480)
        , ball(glm::vec2(piksel::Rng::getInstance().random(2.0f * BRICK_WIDTH, width - 2.0f * BRICK_WIDTH), height / 2 + 60), glm::vec2(BALL_RADIUS, BALL_RADIUS), glm::vec2(piksel::Rng::getInstance().random(-BALL_SPEED, BALL_SPEED), BALL_SPEED))
        , paddle(glm::vec2(width / 2, height - BRICK_WIDTH - PADDLE_HEIGHT / 2), glm::vec2(PADDLE_WIDTH / 2.0f, PADDLE_HEIGHT / 2.0f), RED)
        , character(glm::vec2(width / 2, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - CHARACTER_HEIGHT / 2.0f), glm::vec2(CHARACTER_WIDTH / 2.0f, CHARACTER_HEIGHT / 2.0f))
        , enemySpawnPointToggle(true)
        , enemySpawnDelayInMillis(ENEMY_MAX_SPAWN_DELAY_IN_MILLIS)
        , state(STARTSCREEN)
        , glitch(false)
        , glitchDuration(1000) {
    }
    void setup();
    void draw(piksel::Graphics& g);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int button);
private:
    piksel::Spritesheet spritesheet;
    Character character;
    Ball ball;
    Paddle paddle;
    std::vector<Brick> bricks;
    std::vector<Enemy> enemies;
    glm::vec2 mousePos;
    piksel::Timer enemySpawnTimer;
    bool enemySpawnPointToggle;
    int enemySpawnDelayInMillis;
    AppState state;
    bool glitch;
    piksel::Timer glitchTimer;
    int glitchDuration;
    std::vector<Firework> fireworks;

    void drawBoundaries(piksel::Graphics& g);
    void drawScores(piksel::Graphics& g);
    void initBricks();
    void reset();
};

#endif /* APP_HPP */
