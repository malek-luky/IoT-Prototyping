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
  mySerial.begin(9600);  //serial port to radio
  Serial.println("Startup");
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  initialize_radio();
  delay(2000);
}

/////////////////////////////////////////////////////////////////
/// RADIO INIT
/////////////////////////////////////////////////////////////////
void initialize_radio() {
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);
  delay(100);  //wait for the RN2xx3's startup message
  mySerial.flush();
  myLora.autobaud();  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.

  //check communication with radio
  String hweui = myLora.hweui();
  while (hweui.length() != 16) {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via Heliumctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join Helium");
  bool join_result = false;
  const char *appEui = "6081F9D09104D7DA";                  // CHANGE HERE
  const char *appKey = "A30E1B3EABF7FCD62749DA00A703AD4A";  // CHANGE HERE
  join_result = myLora.initOTAA(appEui, appKey);

  while (!join_result) {
    Serial.println("Unable to join. Are your keys correct, and do you have Helium coverage?");
    delay(10000);  //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined Helium");
}

void loop() {
  currentTime = millis();                    // Read current time
  elapsedTime = currentTime - previousTime;  // Calculate elapsed time

  /////////////////////////////////////////////////////////////////
  /// DOWNLINK
  /////////////////////////////////////////////////////////////////
  if (elapsedTime >= 10000) {              // If 10 second has passed, do the downlink
    String rnCommand = "mac tx uncnf 1 ";  // Creating tx command for RN2483 module. Unconfirmed tx on port 1
    mySerial.println(rnCommand);           // Sending command to RN2483 over serial connection
    while (mySerial.available() > 0)       // Checking if there is any serial data coming in from RN2483
    {
      int incomingByte = mySerial.read();  // Read the incoming bytes from RN2483 module, one by one:
      Serial.write(incomingByte);          // Writes out the byte as a readable ASCII character, one by one
    }
    previousTime = currentTime;  // Update time
  }

  /////////////////////////////////////////////////////////////////
  /// BUZZER
  /////////////////////////////////////////////////////////////////
  buttonState = digitalRead(buttonPin);  // Read button state


  // RECEIVER LOGIC
  if (incomingByte == 31) {
    if (alarmEnabled == false) {
      alarmEnabled == true;
      tone(buzzerPin, 1000);  // Play a default tone
    }
  }

  else if (incomingByte == 30) {
    alarmEnabled = false;
    noTone(buzzerPin);  // Stop the buzzer
  }

  else {
    Serial.println("Unknown Message Received");
  }

  // BUZZER LOGIC
  if (buttonState == LOW) {  // button pressed
    noTone(buzzerPin);       // Stop the buzzer
  }

  delay(100);  // Add a short delay for stability
}
