#include <IRremote.h> // >v3.0.0
                                            
#define PIN_SEND 3

#define RAW_DATA_LEN 350
#define JVCPower 0x80C0
IRsend irsend;

#define movementDetection  HIGH

#define LEDon              HIGH
#define LEDoff             LOW

#define enabled            true
#define disabled           false

//*********************************************************************
const byte heartbeatLED  = 13;
const byte  led          = 8;
const byte  pir          = 2;

bool autoOffFlag         = disabled;

byte lastPIRstate        = !movementDetection;
byte val;
byte state               = 0;
byte LEDstate            = LEDoff;


int sensorPin = A0; // select the input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor


//Timing stuff
const unsigned long timeoutDuration   = 10000;            //10 seconds for testing
//const unsigned long timeoutDuration = 15 * 60 * 1000ul; //15 minutes

unsigned long heartbeatMillis;
unsigned long switchMillis;
unsigned long autoMillis;

//*********************************************************************
void setup()
{
  pinMode(heartbeatLED, OUTPUT);

  //turn the light OFF
  digitalWrite(led, LEDoff);
  pinMode(led, OUTPUT);

  pinMode(pir, INPUT);
  IrSender.begin(PIN_SEND); // Initializes IR sender
  Serial.begin(9600);


} //END of setup()

//*********************************************************************
void loop()
{
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println(sensorValue); //prints the values coming from the sensor on the screen
  
  delay(500); 

  
      //***********************
      //time to toggle the heartbeatLED ?
      if (millis() - heartbeatMillis >= 500)
      {
        //restart the TIMER
        heartbeatMillis = millis();
    
        //toggle the LED
        digitalWrite(heartbeatLED, !digitalRead(heartbeatLED));

      }
    
      //***********************
      //time to check the switches ?
      if (millis() - switchMillis >= 50)
      {
        //restart the TIMER
        switchMillis = millis();
                  

    
        checkSwitches();
      }
    
      //***********************
      //if enabled, has the TIMER reached the timeout interval ?
      if (autoOffFlag == enabled && millis() - autoMillis >= timeoutDuration)
      {
        //disable the TIMER
        autoOffFlag = disabled;
    
        //toggle the LED light
        LEDstate = LEDoff;
    
    
        //turn the light OFF
        digitalWrite(led, LEDoff);
    
        //send IR signal
       uint16_t irSignal[115] = {6592 ... 632};
    
    
      
      
        if (sensorValue > 800)
        {
          IrSender.sendRaw(irSignal, 279 / sizeof(irSignal[0]), 38);
          Serial.println(F("AC Switched Off"));
        }
      }





} //END of loop()


//*********************************************************************
void checkSwitches()
{
  //*****************************************    PIR
  //
  byte state = digitalRead(pir);
  

  //***********************
  //has the PIR changed state ?
  if (lastPIRstate != state)
  {
    //update to the new state
    lastPIRstate = state;

    //************
    //has the PIR detected movement ?
    if (state == movementDetection)
    {
      //toggle the LED light
      LEDstate = LEDon;

      digitalWrite(led, LEDstate);

      if (state == movementDetection)
      {
        //enable the TIMER
        autoOffFlag = enabled;

        //start the TIMER
        autoMillis = millis();


        
      }

      else
      {
        //disable the TIMER but keep the LED on
        digitalWrite(led, LEDon);
        autoOffFlag = disabled;


      }

    }

  } //END of  if (lastPIRstate != state)


} //END of  checkSwitches()

//*********************************************************************