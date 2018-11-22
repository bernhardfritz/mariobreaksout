#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "particle.hpp"
#include <piksel/graphics.hpp>
#include <glm/glm.hpp>
#include <vector>

class Firework {
public:
    Particle rocket;
    bool exploded;
    std::vector<Particle> particles;

    Firework(glm::vec2 position);

    void applyForce(glm::vec2 force);
    void update();
    void explode();
    void draw(piksel::Graphics& g);
    bool isDone();
};

#endif /* FIREWORK_HPP */
