#include <Arduino.h>

class IndicatorLights{
    int _RED_LED;
    int _GREEN_LED;
    int _BLUE_LED;

    public:
        // Define the indicator RGB Light Pins
        IndicatorLights(int redLed, int greenLed, int blueLed){
            _RED_LED = redLed;
            _GREEN_LED = greenLed;
            _BLUE_LED = blueLed;

            pinMode(_RED_LED, OUTPUT);
            pinMode(_GREEN_LED, OUTPUT);
            pinMode(_BLUE_LED, OUTPUT);
        }

        void red(){
            digitalWrite(_RED_LED, LOW);
            digitalWrite(_GREEN_LED, HIGH);
            digitalWrite(_BLUE_LED, HIGH);
        }

        void green(){
            digitalWrite(_RED_LED, HIGH);
            digitalWrite(_GREEN_LED, LOW);
            digitalWrite(_BLUE_LED, HIGH);
        }

        void blue(){
            digitalWrite(_RED_LED, HIGH);
            digitalWrite(_GREEN_LED, HIGH);
            digitalWrite(_BLUE_LED, LOW);
        }
};
