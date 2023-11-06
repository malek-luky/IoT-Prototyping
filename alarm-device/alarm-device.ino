const int buttonPin = 13;
const int buzzerPin = A1;
int buttonState = 0;
bool alarmEnabled = true; 

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read button state

  if (buttonState == LOW) { // button pressed
    noTone(buzzerPin); // Stop the buzzer
    alarmEnabled = false; 
  }

  if (alarmEnabled) {
    tone(buzzerPin, 1000); // Play a default tone
  }

  delay(100); // Add a short delay for stability
}
