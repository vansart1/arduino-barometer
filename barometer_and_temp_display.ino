#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <SoftwareSerial.h>

/*
Tool to display barometric pressure and temperature on display
Uses BMP180 Sensor for pressure and ________ for display

Written by Victor Ansart May 2018

########## Connections: #################
Connecting Sensor:
  Connect SCL to A5
  Connect SDA to A4
  Connect Vin to 3.3 VDC
  Connect GND to ground

Connecting Display:
  Connect RX to D11
  Connect Vin to 5 VDC
  Connect GND to ground

Connecting Buttons:
  Dim Up Button to D2 and ground (each side of button)
  Dim Down Button to D3 and ground (each side of button)

NOTE: Uses LCD display from here: https://github.com/jimblom/Serial-LCD-Kit/wiki/Serial-Enabled-LCD-Kit-Datasheet  
*/


//############## INITIALIZATION ###########

//calibration for pressure in sensor compared to known atmospheric pressure in area
//  - this is obtained by comparing the raw pressure (ie when pressure_calibration=0)
//    to a known pressure in the area from a weather station
const float pressure_calibration = -3.0;


const int up_pin=2;
const int down_pin=3;

volatile int dim_level=25;

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

SoftwareSerial displaySerial(10, 11); // RX, TX

//############## SETUP ###########
void setup() 
{
  //start serials
  Serial.begin(9600);
  displaySerial.begin(9600);


  //initialize sensor
  if(!bmp.begin())
  {
    Serial.print("Error initializing the sensor!");
    displaySerial.println("ERROR");
    while(1);
  }

  //set up interrupts
  pinMode(up_pin, INPUT_PULLUP);    // initialize pin as an input w/ pullup
  pinMode(down_pin, INPUT_PULLUP);    // initialize pin as an input w/ pullup
  attachInterrupt(digitalPinToInterrupt(up_pin), up_ISR, FALLING); //set up interrupt
  attachInterrupt(digitalPinToInterrupt(down_pin), down_ISR, FALLING); //set up interrupt


  
  delay(1000); //wait a second for display to "boot"
}


//############## LOOP ###########
void loop() 
{
  //create and get latest sensor event
  sensors_event_t event;
  bmp.getEvent(&event);

  //if succesfully received event
  if (event.pressure)     //if succesfully received event
  {
    //get pressure
    float pressure = event.pressure;
    pressure += pressure_calibration;   //calibrate pressure

    //get temp
    float tempC;
    bmp.getTemperature(&tempC); 
    float tempF = ( tempC * (9.0 / 5.0) ) + 32.0;   //convert from C to F

    //print to serial output
    Serial.print("Pressure:    ");
    Serial.print(pressure);
    Serial.println(" hPa");
    Serial.print("Temperature:    ");
    Serial.print(tempF);
    Serial.println(" F");

    //set up char arrays for printing
    char line1[30] = "Press:   9999.99";
    char line2[30] = "Temp:     99.99F";

    //convert pressure to string and create line1
    char buff[10] = "";
    dtostrf(pressure, 3, 2, buff);  //convert float to char array
    sprintf(line1,"Press:   %s", buff); //create line

    //convert temp to string and create line2
    dtostrf(tempF, 3, 2, buff);   //convert float to char array
    sprintf(line2,"Temp:    %s F", buff); //create line

   // displaySerial.write(0x80);
   // displaySerial.write(1);


    //send bytes to clear display
    displaySerial.write(254);
    displaySerial.write(1);

    //send data to dsiplay
    displaySerial.write(line1);
    displaySerial.write(line2);

  }
  else    //event not successfully received
  {
    Serial.print("Error reading the sensor!");
    displaySerial.println("ERROR");
  }
  Serial.println(dim_level);
  
  delay( 1 * 1000 );
}


//############## INTERRUPTS ###########

void up_ISR() 
{
  dim_level+=2;
  if ( dim_level > 50 )
  {
    dim_level=50;
  }
    displaySerial.write(0x80);
    displaySerial.write(dim_level);
}

void down_ISR() 
{
  dim_level-=2;
  if ( dim_level < 1 )
  {
    dim_level=1;
  }
    displaySerial.write(0x80);
    displaySerial.write(dim_level);
}

