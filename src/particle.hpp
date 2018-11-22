#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <piksel/graphics.hpp>
#include <glm/glm.hpp>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    int age;
    int lifespan;
    float mass;
    glm::vec4 color;

    Particle()
        : position(glm::vec2(0.0f))
        , velocity(glm::vec2(0.0f))
        , acceleration(glm::vec2(0.0f))
        , age(0)
        , lifespan(-1)
        , mass(1.0f)
        , color(glm::vec4(1.0f)) {
    }

    void applyForce(glm::vec2 force);
    void update();
    void draw(piksel::Graphics& g);
    bool isDead();
};

#endif /* PARTICLE_HPP */