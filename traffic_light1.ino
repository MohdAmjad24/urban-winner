#define RED_LED 7
#define YELLOW_LED 6
#define GREEN_LED 5
#define BUTTON 2

void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);  // Internal pull-up resistor enabled

    // Turn all LEDs OFF initially
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
}

void loop() {
    // If the button is pressed, activate emergency mode
    if (digitalRead(BUTTON) == HIGH) {
        emergencyStop();
    } else {
        normalTrafficCycle();
    }
}

// Normal Traffic Light Sequence
void normalTrafficCycle() {
    setLights(HIGH, LOW, LOW);  // Green ON
    delayWithEmergencyCheck(5000);

    setLights(LOW, HIGH, LOW);  // Yellow ON
    delayWithEmergencyCheck(3000);

    setLights(LOW, LOW, HIGH);  // Red ON
    delayWithEmergencyCheck(3000);
}

// Function to turn LEDs ON/OFF
void setLights(bool green, bool yellow, bool red) {
    digitalWrite(GREEN_LED, green);
    digitalWrite(YELLOW_LED, yellow);
    digitalWrite(RED_LED, red);
}

// Function to handle delays while checking button press
void delayWithEmergencyCheck(int time) {
    for (int i = 0; i < time / 100; i++) {
        if (digitalRead(BUTTON) == HIGH) {
            emergencyStop();
            return;
        }
        delay(100);
    }
}

// Emergency Stop (Red LED ON, Others OFF)
void emergencyStop() {
    setLights(LOW, LOW, HIGH);  // Only Red LED ON

    // Keep red LED ON while button is pressed
    while (digitalRead(BUTTON) == HIGH);

    // When button is released, return to normal cycle
}
