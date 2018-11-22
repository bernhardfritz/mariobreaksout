#ifndef DEFINES_HPP
#define DEFINES_HPP

#define BALL_RADIUS 4.0f
#define BALL_SPEED 4.0f
#define BRICK_WIDTH 32.0f
#define BRICK_HEIGHT 12.0f
#define PADDLE_WIDTH 2.0f * BRICK_WIDTH
#define PADDLE_HEIGHT 2.0f * BALL_RADIUS
#define CHARACTER_WIDTH 16.0f
#define CHARACTER_HEIGHT 32.0f
#define CHARACTER_RUN_ACCELERATION glm::vec2(0.15f, 0.0f)
#define CHARACTER_JUMP_ACCELERATION glm::vec2(0.0f, -1.25f)
#define CHARACTER_MAX_VELOCITY glm::vec2(2.5f, 4.0f)
#define CHARACTER_ANIMATION_MIN_MILLIS_PER_FRAME 2 * 17
#define CHARACTER_ANIMATION_MAX_MILLIS_PER_FRAME 8 * 17
#define PORTAL_WIDTH 64.0f
#define PORTAL_HEIGHT PORTAL_WIDTH
#define PORTAL_DURATION 350.0f
#define ENEMY_WIDTH 16.0f
#define ENEMY_HEIGHT 16.0f
#define ENEMY_BASE_VELOCITY glm::vec2(1.0f, 0.0f)
#define ENEMY_MAX_VELOCITY glm::vec2(1.0f, 4.0f)
#define ENEMY_MIN_SPAWN_DELAY_IN_MILLIS 1000
#define ENEMY_MAX_SPAWN_DELAY_IN_MILLIS 4000
#define GRAVITY glm::vec2(0.0f, 0.2f)
#define RED glm::vec4(198 / 255.0f, 73 / 255.0f, 75 / 255.0f, 1.0f)
#define ORANGE glm::vec4(196 / 255.0f, 108 / 255.0f, 64 / 255.0f, 1.0f)
#define BROWN glm::vec4(179 / 255.0f, 122 / 255.0f, 55 / 255.0f, 1.0f)
#define YELLOW glm::vec4(162 / 255.0f, 161 / 255.0f, 53 / 255.0f, 1.0f)
#define GREEN glm::vec4(76 / 255.0f, 159 / 255.0f, 76 / 255.0f, 1.0f)
#define BLUE glm::vec4(67 / 255.0f, 77 / 255.0f, 197 / 255.0f, 1.0f)
#define GRAY glm::vec4(142 / 255.0f, 142 / 255.0f, 142 / 255.0f, 1.0f)
#define BLACK glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define WHITE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define CYAN glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
#define PINK glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)

#endif /* DEFINES_HPP */