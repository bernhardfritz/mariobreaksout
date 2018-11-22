#include "particle.hpp"

void Particle::applyForce(glm::vec2 force) {
    acceleration += force * mass;
}

void Particle::update() {
    velocity += acceleration;
    position += velocity;
    acceleration = glm::vec2(0.0f);
    if (age < lifespan) {
        age++;
    }
}

void Particle::draw(piksel::Graphics& g) {
    if (lifespan == -1) {
        g.stroke(color);
    } else {
        float pct = 1.0f - age / (float) lifespan;
        g.stroke(glm::vec4(color.r, color.g, color.b, pct));
    }
    g.strokeWeight(mass * 8.0f);
    g.point(position.x, position.y);
}

bool Particle::isDead() {
    return age == lifespan;
}