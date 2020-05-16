
#ifndef SEABATTLE_CANIMATION_H
#define SEABATTLE_CANIMATION_H

#include "CCursor.h"

class CAnimation {
    bool is_animating;
public:
    CAnimation() {
        is_animating = false;
    }
    CAnimation(bool is_animating) {
        this->is_animating = is_animating;
    }

    void set_is_animating(bool);
    bool get_is_animating();
    virtual void update_state(int frame_number) {

    };
};


#endif //SEABATTLE_CANIMATION_H
