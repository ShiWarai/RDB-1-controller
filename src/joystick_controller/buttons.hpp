#pragma once

class ClickableButton {
public:
    bool state();
    bool press();
    bool release();
    bool turn(bool direction);
private:
    bool __state = false;
    bool __buttonPressed = false;
};

class ButtonWithState {
public:
    ButtonWithState(bool state = false);

    bool state();
    bool turn(bool direction);
private:
    bool __state;
    bool __buttonPressed = false;
};