#include "app.hpp"
#include <iomanip>
#include <sstream>

void App::setup() {
    spritesheet.load("data/spritesheet.json");
    character.setup(spritesheet, height);
    initBricks();
    enemySpawnTimer.reset(millis());
    ball.setup(millis());
}

void App::initBricks() {
    std::vector<glm::vec4> colors = { RED, ORANGE, BROWN, YELLOW, GREEN, BLUE };
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 18; col++) {
            Brick brick(glm::vec2(BRICK_WIDTH + BRICK_WIDTH / 2.0f + col * BRICK_WIDTH, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 + BRICK_HEIGHT / 2.0f + row * BRICK_HEIGHT), glm::vec2(BRICK_WIDTH / 2.0f, BRICK_HEIGHT / 2.0f), colors[row]);
            bricks.push_back(brick);
        }
    }
}

void App::draw(piksel::Graphics& g) {
    if (state != STARTSCREEN && state != VICTORY) {
        if (enemySpawnTimer.getElapsedTimeInMillis(millis()) > enemySpawnDelayInMillis) {
            enemySpawnTimer.reset(millis());
            if (enemySpawnPointToggle) {
                Enemy enemy(glm::vec2(ENEMY_WIDTH / 2.0f, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - ENEMY_HEIGHT / 2.0f), glm::vec2(ENEMY_WIDTH / 2.0f, ENEMY_HEIGHT / 2.0f), ENEMY_BASE_VELOCITY);
                enemy.setup(spritesheet);
                enemies.push_back(enemy);
            } else {
                Enemy enemy(glm::vec2(width - ENEMY_WIDTH / 2.0f, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - ENEMY_HEIGHT / 2.0f), glm::vec2(ENEMY_WIDTH / 2.0f, ENEMY_HEIGHT / 2.0f), -ENEMY_BASE_VELOCITY);
                enemy.setup(spritesheet);
                enemies.push_back(enemy);
            }
            enemySpawnPointToggle = !enemySpawnPointToggle;
        }
    }
    g.background(BLACK);
    g.noStroke();
    if (state != GAMEOVER && state != VICTORY) {
        for (Brick& brick : bricks) {
            brick.draw(g);
        }
        paddle.update(BRICK_WIDTH, width - BRICK_WIDTH, mousePos);
        paddle.draw(g);
    }
    if (state != STARTSCREEN) {
        std::vector<Enemy>::iterator it = enemies.begin();
        while (it != enemies.end()) {
            Enemy& enemy = *it;
            enemy.update(width, paddle, bricks, character, millis(), state == VICTORY || state == GAMEOVER);
            if ((enemy.getTimeOfDeathInMillis() > -1 && millis() - enemy.getTimeOfDeathInMillis() > 3000)
                || enemy.isOutOfBounds(0, width, BRICK_WIDTH + BRICK_WIDTH, height)) {
                it = enemies.erase(it);
            } else {
                enemy.draw(g);
                it++;
            }
        }
    }
    if (state == VICTORY) {
        g.push();
        if (piksel::Rng::getInstance().random(0, 1) < 0.1f) {
            Firework firework(glm::vec2(piksel::Rng::getInstance().random(0, width), height));
            fireworks.push_back(firework);
        }
        for (Firework& firework : fireworks) {
            firework.applyForce(GRAVITY);
            firework.update();
            firework.draw(g);
        }
        std::vector<Firework>::iterator it = fireworks.begin();
        while (it != fireworks.end()) {
            if ((*it).isDone()) {
                it = fireworks.erase(it);
            } else {
                it++;
            }
        }
        g.pop();
    }
    drawBoundaries(g);
    character.update(BRICK_WIDTH, width - BRICK_WIDTH, BRICK_WIDTH + BRICK_WIDTH, height, paddle, bricks, millis(), width / 2, state == VICTORY || state == GAMEOVER);
    character.draw(g);
    if (state == GAME) {
        ball.update(BRICK_WIDTH, width - BRICK_WIDTH, BRICK_WIDTH + BRICK_WIDTH, height, paddle, bricks, enemySpawnDelayInMillis, width, height, millis());
        ball.draw(g);
    }
    drawScores(g);
    if (character.getLives() <= 0 || ball.getLives() <= 0) {
        state = GAMEOVER;
        bricks.clear();
    } else if (bricks.empty()) {
        state = VICTORY;
    }
    if (state == STARTSCREEN || state == GAMEOVER || state == VICTORY) {
        g.push();

        g.textSize(42);
        float hw;
        switch (state) {
            case STARTSCREEN:
                hw = 66;
                break;
            case GAMEOVER:
                hw = 166;
                break;
            case VICTORY:
            default:
                hw = 52;
                break;
        }
        float hh = height / 2 - 12;
        glm::vec4 pink = PINK;
        pink.a = 0.5;
        glm::vec4 cyan = CYAN;
        cyan.a = 0.5;
        if (glitchTimer.getElapsedTimeInMillis(millis()) > glitchDuration) {
            if (glitch) {
                glitchDuration = int(piksel::Rng::getInstance().random(300, 1500));
            } else {
                glitchDuration = int(piksel::Rng::getInstance().random(150, 300));
            }
            glitchTimer.reset(millis());
            glitch = !glitch;
        }
        if (glitch) {
            g.scale(piksel::Rng::getInstance().random(0.99f, 1.01f), piksel::Rng::getInstance().random(0.99f, 1.01f));
        }
        g.translate(hw + (glitch ? piksel::Rng::getInstance().random(-3.0f, 3.0f) : 0), hh);
        std::string str;
        switch(state) {
            case STARTSCREEN:
                str = "Mario breaks out";
                break;
            case GAMEOVER:
                str = "GAME OVER";
                break;
            case VICTORY:
            default:
                str = "CONGRATULATIONS!";
                break;
        }
        g.fill(BLACK);
        g.text(str, 0, 3);
        if (glitch) {
            g.fill(pink);
            g.text(str, -3, 0);
            g.fill(cyan);
            g.text(str, 3, 0);
        }
        g.fill(WHITE);
        g.text(str, 0, 0);
        g.pop();
        if ((g.millis() / 1000) % 2 == 0) {
            g.text(state == STARTSCREEN ? "PRESS ENTER TO START" : "PRESS ENTER TO RESTART", width / 2 - (state == STARTSCREEN ? 95 : 105), height / 2 + 42);
        }
    }
}

void App::keyPressed(int key) {
    switch (key) {
        case 263: // left arrow
        case 'A':
            character.startRunning(true);
            break;
        case 262: // right arrow
        case 'D':
            character.startRunning(false);
            break;
        case 265: // up arrow
        case 'W':
        case ' ':
            character.startJumping();
            break;
        case 264: // down arrow
        case 'S':
            character.startDucking();
            // bricks.clear(); // for debug
            break;
        case 257: // enter
            if (state == STARTSCREEN || state == GAMEOVER || state == VICTORY) {
                reset();
                state = GAME;
            }
    }
}

void App::keyReleased(int key) {
    switch (key) {
        case 263: // left arrow
        case 'A':
            character.stopRunning(true);
            break;
        case 262: // right arrow
        case 'D':
            character.stopRunning(false);
            break;
        case 265: // up arrow
        case 'W':
        case ' ':
            character.stopJumping();
            break;
        case 264: // down arrow
        case 'S':
            character.stopDucking();
            break;
    }
}

void App::mouseMoved(int x, int y) {
    mousePos.x = x;
    mousePos.y = y;
}

void App::mousePressed(int button) {
    /* for debug */
    // std::vector<Brick>::iterator it = bricks.begin();
    // while (it != bricks.end()) {
    //     Brick& brick = *it;
    //     if (brick.getLeft() <= mousePos.x && mousePos.x <= brick.getRight()
    //         && brick.getTop() <= mousePos.y && mousePos.y <= brick.getBottom()) {
    //         it = bricks.erase(it);
    //     } else {
    //         it++;
    //     }
    // }
}

void App::drawBoundaries(piksel::Graphics& g) {
    g.push();
    g.rectMode(piksel::DrawMode::CORNERS);
    g.fill(GRAY);
    g.noStroke();
    g.rect(0, BRICK_WIDTH, width, BRICK_WIDTH + BRICK_WIDTH);
    g.rect(0, BRICK_WIDTH + BRICK_WIDTH, BRICK_WIDTH, height);
    g.rect(width - BRICK_WIDTH, BRICK_WIDTH + BRICK_WIDTH, width, height);
    g.imageMode(piksel::DrawMode::CORNER);
    spritesheet.sprites[38].draw(g, 0, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - 32, 32, 32, true);
    spritesheet.sprites[38].draw(g, width - 32, BRICK_WIDTH + BRICK_WIDTH + 48 + 60 - 32, 32, 32, false);
    g.pop();
}

void App::drawScores(piksel::Graphics& g) {
    g.push();
    spritesheet.sprites[39].draw(g, 0, 0, 32, 32);
    spritesheet.sprites[40].draw(g, width - 80, 0, 32, 32);
    g.textSize(16);
    g.text("x0" + std::to_string(character.getLives()), 32, 24);
    g.text("x0" + std::to_string(ball.getLives()), width - 47, 24);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(6) << ball.getScore();
    g.text(ss.str(), width / 2 - 39, 24);
    g.pop();
}

void App::reset() {
    ball.reset(width, height, millis());

    character.reset(width, state != STARTSCREEN);

    bricks.clear();
    initBricks();

    enemies.clear();
    enemySpawnPointToggle = true;
    enemySpawnTimer.reset(millis());
}
