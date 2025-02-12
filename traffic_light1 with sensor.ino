#define RED_LED1 7
#define YELLOW_LED 6
#define GREEN_LED 5
#define BUTTON 2
#define BLUE_LED 12
#define RED_LED2 10
#define TRIG_PIN 9
#define ECHO_PIN 8

#define DETECTION_DISTANCE 100  // Distance threshold in cm

void setup() {
    pinMode(RED_LED1, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_LED2, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Turn all LEDs OFF initially
    digitalWrite(RED_LED1, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED2, LOW);
    Serial.begin(9600);
}

void loop() {
    float distance = getDistance();  // Get ultrasonic sensor reading

    // If emergency button is pressed or an object is detected within 30cm, stop
  if (digitalRead(BUTTON) == HIGH ){
    	emergencyStop();
  }else if (distance <= DETECTION_DISTANCE){
    	normalTrafficCycle();   
    } else {
      	
        normalTrafficCycle();
    }
}

// Normal Traffic Light Sequence
void normalTrafficCycle() {
    setLights(HIGH, LOW, LOW, LOW, HIGH);  // Green ON
    delayWithEmergencyCheck(3000);

    setLights(LOW, HIGH, LOW, LOW, HIGH);  // Yellow ON
    delayWithEmergencyCheck(1000);

    setLights(LOW, LOW, HIGH, HIGH, LOW);  // Red ON
    delayWithEmergencyCheck(3000);
}

// Function to measure distance using Ultrasonic Sensor
float getDistance() {
    long duration;
    float distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * 0.0343) / 2;  // Convert time to distance

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    return distance;
}

// Function to turn LEDs ON/OFF
void setLights(bool green, bool yellow, bool red, bool blue, bool orange) {
    digitalWrite(GREEN_LED, green);
    digitalWrite(YELLOW_LED, yellow);
    digitalWrite(RED_LED1, red);
    digitalWrite(BLUE_LED, blue);
    digitalWrite(RED_LED2, orange);
}

// Function to handle delays while checking button press
void delayWithEmergencyCheck(int time) {
    for (int i = 0; i < time / 100; i++) {
        if (digitalRead(BUTTON) == HIGH || getDistance() >= DETECTION_DISTANCE) {
            emergencyStop();
            return;
        }
        delay(100);
    }
}

// Emergency Stop (Red LED ON, Others OFF)
void emergencyStop() {
    setLights(LOW, LOW, HIGH, HIGH, LOW);  // Only Red LED ON

    // Stay in emergency mode until button is released 
    while (digitalRead(BUTTON) == HIGH || getDistance() >= DETECTION_DISTANCE);

    delay(500);  // Small delay to avoid immediate restart
}
