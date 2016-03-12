byte piState = 0;
const byte pi_UART_TX = 0;      // Raspberry Pi UART TX pin on digital input 0
const byte regulator_state = 2; // LM2596 on/off on digital output 2
const byte power_switch = 1;    // Power button on digital input 1
const byte rgb_led_red = 3;     // Red led of RGB led, for indicator LED
const byte rgb_led_green = 4;   // Green led of RGB led   ----- " ------

unsigned long interval = 300;

unsigned long previousMillis = 0;

byte checks = 0;
byte piOn = 0;

// the setup routine runs once when you press reset:
void setup() {
    pinMode(regulator_state, OUTPUT);
    pinMode(pi_UART_TX, INPUT);
    pinMode(power_switch, INPUT);
    pinMode(rgb_led_red, OUTPUT);
    pinMode(rgb_led_green, OUTPUT);
    digitalWrite(regulator_state, HIGH); // start up with regulator OFF
    digitalWrite(rgb_led_green, HIGH); // green LED always on
}

// the loop routine runs over and over again forever:
void loop() {
    unsigned long currentMillis = millis();

    if(digitalRead(power_switch) == HIGH) {
      digitalWrite(regulator_state, LOW);
      // let the Pi get some time to boot up
      delay(2000);
    }
    
    piState = digitalRead(pi_UART_TX);

    if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;

        if(piState == LOW) {
            checks++;
            if (checks == 3) { // after 3 consecutive checks where Pi UART is down
                checks = 0;
                digitalWrite(regulator_state, HIGH); // set the regulator HIGH, meaning off
                piOn = 0;
            }
        } else {
            piOn = 1;
            checks = 0; // reset check counter if Pi is on
        }
    }
    
    if(piOn == 1) {
      digitalWrite(rgb_led_red, LOW);
    } else {
      digitalWrite(rgb_led_red, HIGH);
    }

}
