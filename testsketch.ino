#include <HITIComm.h>
#include <Arduino.h>
#include <SoftPWM.h>

//Car Stuff
/*
 *  [0]--|||--[1]
 *   |         |
 *   |         |
 *   |         |
 *   |         |
 *  [3]-------[2]
 */
/** Set the pins for the motors */
#define MOTOR_PINS \
  (uint8_t[8]) { \
    3, 4, 5, 6, A3, A2, A1, A0 \
  }
/** Set the positive and negative directions for the motors */
#define MOTOR_DIRECTIONS \
  (uint8_t[4]) { \
    1, 0, 0, 1 \
  }

#define MOTOR_POWER_MIN 28  // 28/255

int8_t power = 80;


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

    // set pin 5 and 6 as PWM Outputs
    HC_outputType(5, PWM);
    HC_outputType(6, PWM);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    // set pin 8 as Servo Output (initial position is 12.8°)
    HC_attachServo(8, 12800);

    // set pin 13 as Digital Output
    pinMode(LED_BUILTIN, OUTPUT);
    
}


void loop()
{
    // communicate with HITIPanel or HITIBrain
    HC_communicate();


    // read photoresistor on Analog Input 0, and convert value in %
    float rawSensorValue = analogRead(0);
    float sensorValueInPercent = HCS_map(rawSensorValue, 0, 1023, 0, 100);



    // use Analog Data 0 as a Metric in HITIPanel
    // => to display the sensor value in %
    HC_analogDataWrite(0, sensorValueInPercent);


    // use Analog Data 1 as a Setpoint in HITIPanel
    // => to set a threshold value for the sensor
    // => and to constrain this value between 0 and 100
    float threshold = HC_analogDataRead_setpoint(1, 0, 100);


    // use Digital Data 0 as an Indicator in HITIPanel
    // => which turns on if the sensor value exceeds the threshold value
    bool indicatorValue = (sensorValueInPercent > threshold);
    HC_digitalDataWrite(0, indicatorValue);


    // use Digital Data 1 as a Switch in HITIPanel
    // => to toggle the servo between 2 positions
    //if (HC_digitalDataRead(1))
        // if Switch is activated, move Servo to position 169.3°
        //HC_servoWrite(8, 169300);
    //else
        // if Switch is deactivated, move Servo back to position 12.8°
     //   HC_servoWrite(8, 12800);


    // use Digital Data 2 as a Button in HITIPanel
    // => to increment a counter
    if (HC_digitalDataRead_click(2))
        // when Button is clicked, increment counter
        clickCounter++;

    // use Analog Data 2 as a Metric in HITIPanel
    // => to display the counter value
    HC_analogDataWrite(2, clickCounter);


}

