#include <HITIComm.h>

// sketch ID
// (used to identify the sketch running on your Arduino)
const char code_name[]    PROGMEM = "My sketch";
const char code_version[] PROGMEM = "1.0.0";

// button click counter
unsigned long clickCounter = 0;


void setup()
{
    // initialize HITIComm library
    HC_begin();

    // set sketch ID
    HC_codeName(code_name);
    HC_codeVersion(code_version);

    // set pin 3 as Digital Input (with internal pull-up)
    pinMode(3, INPUT_PULLUP);

    // set pin 9-13 as Servo Output (initial position is 12.8°)
    HC_attachServo(9, 10000);
    HC_attachServo(10, 10000);
    HC_attachServo(11, 10000);
    HC_attachServo(12, 10000);
    HC_attachServo(13, 10000);
    
}

   
 bool isHandGrabbing = false;
 bool isHandPointing = false;

void loop()
{
    // communicate with HITIPanel or HITIBrain
    HC_communicate();


    //Small delay to prevent closing and opening instantly after one another


  //FINGER MAPPING GUIDE (As of 9/13/24)

  /*
  Thumb: PIN 9
  Index: PIN 10
  Middle: PIN 11
  Ring: PIN 12
  Pinky: Pin 13
  */

  //THIS IS FOR BASIC OPEN CLOSE GESTURE
  //Basically, if digital data 0 in the data panel on hiti brain goes to 1
  //If hand is open, close it, if it's closed then open it.
  if(HC_digitalDataRead(3)) {
       if(isHandGrabbing == false) {
               HC_servoWrite(9, 180000);
               HC_servoWrite(10, 180000);
               HC_servoWrite(11, 180000);
               HC_servoWrite(12, 180000);
               HC_servoWrite(13, 180000);
               isHandGrabbing = true;
               isHandPointing = false;               
       } else {
               HC_servoWrite(9, 0);
               HC_servoWrite(10, 0);
               HC_servoWrite(11, 0);
               HC_servoWrite(12, 0);
               HC_servoWrite(13, 0);   
               isHandGrabbing = false;   
               isHandPointing = false;
       }
       delay(1000);
}



  //POINTING GESTURE
  if(HC_digitalDataRead(4)) {
       if(isHandPointing == false) {
               HC_servoWrite(9, 180000);
               HC_servoWrite(10, 0);
               HC_servoWrite(11, 180000);
               HC_servoWrite(12, 180000);
               HC_servoWrite(13, 180000);
               isHandPointing = true;
               isHandGrabbing = false;
       } else {
               HC_servoWrite(9, 0);
               HC_servoWrite(10, 0);
               HC_servoWrite(11, 0);
               HC_servoWrite(12, 0);
               HC_servoWrite(13, 0);    
               isHandPointing = false;    
               isHandGrabbing = false; 
       }
       delay(1000);
}

    // use Digital Data 2 as a Button in HITIPanel
    // => to increment a counter
    if (HC_digitalDataRead_click(2))
        // when Button is clicked, increment counter
        clickCounter++;

    // use Analog Data 2 as a Metric in HITIPanel
    // => to display the counter value
    HC_analogDataWrite(2, clickCounter);
}

