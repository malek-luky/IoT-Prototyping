/*
 * Author: Dennis Ruigrok and JP Meijers
 * Date: 2017-01-16
 *
 * This program is meant to be used with an Arduino UNO or NANO, conencted to an RNxx3 radio module.
 * It will most likely also work on other compatible Arduino or Arduino compatible boards,
 * like The Things Uno, but might need some slight modifications.
 *
 * Transmit a one byte packet via TTN, using confirmed messages,
 * waiting for an acknowledgement or a downlink message.
 *
 * CHECK THE RULES BEFORE USING THIS PROGRAM!
 *
 * CHANGE ADDRESS!
 * Change the device address, network (session) key, and app (session) key to the values
 * that are registered via the TTN dashboard.
 * The appropriate line is "myLora.initABP(XXX);" or "myLora.initOTAA(XXX);"
 * When using ABP, it is advised to enable "relax frame count".
 *
 * Connect the RN2xx3 as follows:
 * RN2xx3 -- Arduino
 * Uart TX -- 10
 * Uart RX -- 11
 * Reset -- 12
 * Vcc -- 3.3V
 * Gnd -- Gnd
 *
 * If you use an Arduino with a free hardware serial port, you can replace
 * the line "rn2xx3 myLora(mySerial);"
 * with     "rn2xx3 myLora(SerialX);"
 * where the parameter is the serial port the RN2xx3 is connected to.
 * Remember that the serial port should be initialised before calling initTTN().
 * For best performance the serial port should be set to 57600 baud, which is impossible with a software serial port.
 * If you use 57600 baud, you can remove the line "myLora.autobaud();".
 *
 */
#include <rn2xx3.h>
#include <SoftwareSerial.h>
#include "base64.hpp"

const int buzzer = A1;
const int button = 8;
int buttonState = 0;
int buttonprevState = 0;
int buttonnextState = 0;

SoftwareSerial mySerial(10, 11); // LoRa serial RX, TX

//create an instance of the rn2xx3 library,
//giving the software serial as port to use
rn2xx3 myLora(mySerial);

// the setup routine runs once when you press reset:
void setup()
{
  pinMode(buzzer, OUTPUT);
  //output LED pin
  pinMode(button, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(57600); //serial port to computer
  mySerial.begin(9600); //serial port to radio
  Serial.println("Startup");

  //establishing connection to backend
  initialize_radio();

  //transmit a startup message
  myLora.tx("TTN Mapper on TTN Enschede node");
  
  buttonprevState = digitalRead(button);
  buttonnextState = digitalRead(button);

  led_off();
  delay(2000);
}

void initialize_radio()
{
  //reset rn2483
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  mySerial.flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  //check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccesful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;


  //OTAA connection credentials
  const char *appEui = "6081F924D81FE16F";
  const char *appKey = "0DE827BDA3DA6C775087F3793825B6AD";
  
  join_result = myLora.initOTAA(appEui, appKey);
  
  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
  

}

// the loop routine runs over and over again forever:
void loop()
{
    buttonState = digitalRead(button);  // reading button state
    if (buttonState == LOW & buttonprevState == HIGH & buttonnextState == HIGH) {  // button pressed
        noTone(buzzer);
        delay(30000);  //buzzer deactiveness period, here - 30 s, adjustable!!!
    }
    led_on();
    String received;
    myLora.tx("!");  //tx sync uplink message for opening rx window
    Serial.print("Received downlink: ");
    received = myLora.getRx();  //write received byte into string variable
    alarmON(); //rx data processing function
    led_off();
    delay(2000);
}
void alarmON() 
{
   if(myLora.getRx().length()==11) {
          Serial.println("PATIENT IS MISSING");
          tone(buzzer, 1000); // send 1KHz sound signal...
          
     } else if(myLora.getRx().length()==3){
          noTone(buzzer);  // do not start the buzzer
          Serial.println("PATIENT IS ON HIS BED");      
     }
      
}


void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}
