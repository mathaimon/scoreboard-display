#include <Arduino.h>


class DisplayBoard{

    private:
        int _LATCH_PIN; // ST_CP Storage Register Clock Input
        int _CLOCK_PIN; // SH_CP Shift Register Clock Input
        int _DATA_PIN;  // DS    Serial Data Input

        // Decimal representation of the binary outputs of the shift register.
        int dec_digits[10] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 123};
        // Array to the recieved score values excluding the leading '1'
        int scoresArray[9];

        void convertToArray(int num){
            int temp = num;
            for(int i = 8; i>=0; i--){
                scoresArray[i] = temp%10;
                temp /= 10;
            }
        }

    public:
        // Define the shift register pins
        DisplayBoard(int latchPin, int clockPin, int dataPin){
            _LATCH_PIN = latchPin;
            _CLOCK_PIN = clockPin;
            _DATA_PIN = dataPin;

            pinMode(_LATCH_PIN, OUTPUT);
            pinMode(_CLOCK_PIN, OUTPUT);
            pinMode(_DATA_PIN, OUTPUT);
        }

        // Display the score on the display board
        void show(int score){
            convertToArray(score);
            digitalWrite(_LATCH_PIN, LOW);
            for(int i=8; i>=0; i--){
                shiftOut(_DATA_PIN, _CLOCK_PIN, LSBFIRST, dec_digits[scoresArray[i]]);
            }
            digitalWrite(_LATCH_PIN, HIGH);

            //Debug Code
            Serial.println("Scores Array....");
            for(int j=0; j<9; j++){
                Serial.print(scoresArray[j]);
                Serial.print(",");
            }
        }
};