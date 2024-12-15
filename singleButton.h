#include <Arduino.h>

class SingleButton
{
private:
    uint8_t buttonState = HIGH;
    uint8_t lastButtonState = HIGH;
    uint32_t debounceDelay = 50;
    uint32_t lastDebounceTime = 0; 
    uint8_t gpio_pin;

public:
    SingleButton(uint8_t pin, uint32_t debounce = 20) {
        pinMode(pin, INPUT_PULLUP);
        gpio_pin = pin;
        debounceDelay = debounce;
    }

    bool read();
};

bool SingleButton::read() {
    bool currentState = digitalRead(gpio_pin);

    if (currentState != lastButtonState) {
        lastDebounceTime = millis();
        lastButtonState = currentState;
    }

    if ((millis() - lastDebounceTime) >= debounceDelay) {
        if (currentState != buttonState) {
            buttonState = currentState;

            if (buttonState == LOW) {
                return true;
            }
        }
    }

    return false;
}

