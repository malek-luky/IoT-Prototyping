#include <rn2xx3.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX, TX
rn2xx3 myLora(mySerial);          //instance of the rn2xx3 library,

// Button + Buzzer
const int buttonPin = 13;
const int buzzerPin = A1;
int buttonState = 0;
bool alarmEnabled = false;
int incomingByte;

// Init Time
unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long elapsedTime = 0;

/////////////////////////////////////////////////////////////////
/// SETUP
/////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);    //serial port to computer
  Serial.println("Startup");
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  delay(2000);
}

void loop() {
  currentTime = millis();                    // Read current time
  elapsedTime = currentTime - previousTime;  // Calculate elapsed time

  /////////////////////////////////////////////////////////////////
  /// BUZZER
  /////////////////////////////////////////////////////////////////
  buttonState = digitalRead(buttonPin);  // Read button state


  // RECEIVER LOGIC
  alarmEnabled == true;
  tone(buzzerPin, 1000);  // Play a default tone


  // BUZZER LOGIC
  if (buttonState == LOW) {  // button pressed
    noTone(buzzerPin);       // Stop the buzzer
  }

  delay(100);  // Add a short delay for stability
}
