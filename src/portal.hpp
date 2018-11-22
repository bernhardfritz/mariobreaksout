#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "aabb.hpp"
#include <piksel/graphics.hpp>
#include <piksel/ext/spritesheet.hpp>
#include <piksel/ext/animation.hpp>
#include <piksel/ext/timer.hpp>

enum PortalState {
    CLOSED = 0,
    OPENING,
    OPEN,
    CLOSING
};

class Portal : public AABB {
public:

    Portal(bool flipVertically) : AABB(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)), flipVertically(flipVertically), state(CLOSED), scale(0.0f) {}
    void setup(piksel::Spritesheet& spritesheet, int height);
    void update(int currentTimeInMillis);
    void draw(piksel::Graphics& g);
    void open(int currentTimeInMillis, float x);
    void close(int currentTimeInMillis);
    PortalState getState();
private:
    piksel::Animation animation;
    bool flipVertically;
    PortalState state;
    float scale;
    piksel::Timer timer;
};

#endif /* PORTAL_HPP */