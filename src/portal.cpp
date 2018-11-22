#include "portal.hpp"
#include "defines.hpp"

void Portal::setup(piksel::Spritesheet& spritesheet, int height) {
    if (flipVertically) {
        animation.frames.push_back(spritesheet.sprites[41]);
        animation.frames.push_back(spritesheet.sprites[42]);
        animation.frames.push_back(spritesheet.sprites[43]);
        animation.frames.push_back(spritesheet.sprites[44]);
        animation.frames.push_back(spritesheet.sprites[45]);
        animation.frames.push_back(spritesheet.sprites[46]);
        animation.frames.push_back(spritesheet.sprites[47]);
        animation.frames.push_back(spritesheet.sprites[48]);
        animation.frames.push_back(spritesheet.sprites[49]);

        center.y = 32.0f;
    } else {
        animation.frames.push_back(spritesheet.sprites[50]);
        animation.frames.push_back(spritesheet.sprites[51]);
        animation.frames.push_back(spritesheet.sprites[52]);
        animation.frames.push_back(spritesheet.sprites[53]);
        animation.frames.push_back(spritesheet.sprites[54]);
        animation.frames.push_back(spritesheet.sprites[55]);
        animation.frames.push_back(spritesheet.sprites[56]);
        animation.frames.push_back(spritesheet.sprites[57]);
        animation.frames.push_back(spritesheet.sprites[58]);

        center.y = height - PORTAL_HEIGHT / 2.0f;
    }

    animation.millisPerFrame = 50;
}

void Portal::update(int currentTimeInMillis) {
    int elapsedTimeInMillis = timer.getElapsedTimeInMillis(currentTimeInMillis);
    switch (state) {
        case CLOSED:
            break;
        case OPENING:
            if (elapsedTimeInMillis > PORTAL_DURATION) {
                state = OPEN;
            } else {
                scale = elapsedTimeInMillis / PORTAL_DURATION;
            }
            break;
        case OPEN:
            close(currentTimeInMillis);
            break;
        case CLOSING:
            if (elapsedTimeInMillis > PORTAL_DURATION) {
                state = CLOSED;
            } else {
                scale = (PORTAL_DURATION - elapsedTimeInMillis) / PORTAL_DURATION;
            }
            break;
    }
}

void Portal::draw(piksel::Graphics& g) {
    if (state == CLOSED) {
        return;
    }
    g.push();
    g.imageMode(piksel::DrawMode::CENTER);
    animation.draw(g, center.x - scale * PORTAL_WIDTH / 2, center.y - scale * PORTAL_HEIGHT / 2 + PORTAL_HEIGHT / 2, scale * PORTAL_WIDTH, scale * PORTAL_HEIGHT, false, flipVertically, true);
    g.pop();
}

void Portal::open(int currentTimeInMillis, float x) {
    if (state != CLOSED) {
        return;    
    }
    state = OPENING;
    timer.reset(currentTimeInMillis);
    center.x = x;
}

void Portal::close(int currentTimeInMillis) {
    if (state != OPEN) {
        return;
    }
    state = CLOSING;
    timer.reset(currentTimeInMillis);
}

PortalState Portal::getState() {
    return state;
}