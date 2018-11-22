#include "firework.hpp"
#include <piksel/ext/rng.hpp>
#include <cmath>

Firework::Firework(glm::vec2 position) {
    rocket.position = position;
    rocket.velocity = glm::vec2(0.0f, piksel::Rng::getInstance().random(-10.0f, -12.0f));
    rocket.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    exploded = false;
}

void Firework::applyForce(glm::vec2 force) {
    if (!exploded) {
        rocket.applyForce(force);
    } else {
        for (Particle& particle : particles) {
            particle.applyForce(force);
        }
    }
}

void Firework::update() {
    if (!exploded) {
        rocket.update();
        if (rocket.velocity.y >= 0.0f) {
            explode();
        }
    } else {
        for (Particle& particle : particles) {
            particle.velocity *= 0.95;
            particle.update();
        }
        std::vector<Particle>::iterator it = particles.begin();
        while(it != particles.end()) {
            if ((*it).isDead()) {
                it = particles.erase(it);
            } else {
                it++;
            }
        }
    }
}

void Firework::explode() {
    piksel::Rng& rng = piksel::Rng::getInstance();
    glm::vec4 color = glm::vec4(rng.random(0.0f, 1.0f), rng.random(0.0f, 1.0f), rng.random(0.0f, 1.0f), 1.0f);
    for (int i = 0; i < 200; i++) {
        Particle particle;
        particle.position = rocket.position;
        particle.mass = rng.random(0.7, 1.0);
        float angle = rng.random(0.0, piksel::TWO_PI);
        float magnitude = rng.random(2.0f, 10.0f);
        particle.velocity = glm::vec2(magnitude * cos(angle), magnitude * sin(angle));
        particle.lifespan = rng.random(90.0f, 110.0f);
        particle.color = color;
        particles.push_back(particle);
    }
    exploded = true;
}

void Firework::draw(piksel::Graphics& g) {
    g.noStroke();
    if (!exploded) {
        rocket.draw(g);
    } else {
        for (Particle& particle : particles) {
            particle.draw(g);
        }
    }
}

bool Firework::isDone() {
    return exploded && particles.empty();
}