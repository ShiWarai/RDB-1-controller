#pragma once

class ClickableButton {
private:
    bool __state = false;
    bool __buttonPressed = false;
public:
    bool state() {
        return __state;
    }

    bool press() {
        if (!__buttonPressed) {
            __state = true;
            __buttonPressed = true;
        }
        else {
            __state = false;
        }

        return __state;
    }

    bool release() {
        __state = false;
        __buttonPressed = false;

        return false;
    }

    bool turn(bool direction) {
        return direction ? this->press() : this->release();
    }
};

class ButtonWithState {
private:
    bool __state;
    bool __buttonPressed = false;
public:
    ButtonWithState(bool state = false) {
        this->__state = state;
    }

    bool state() {
        return __state;
    }

    bool turn(bool direction) {
        if (direction) {
            if (!__buttonPressed)
            {
                __state = !__state;
            }
            __buttonPressed = true;
        }
        else {
            __buttonPressed = false;
        }

        return __state;
    }
};