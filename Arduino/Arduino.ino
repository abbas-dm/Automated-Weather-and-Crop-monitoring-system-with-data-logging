/*************************************************************************/
                              //BPT Project 
/*************************************************************************/

// Short note on the project for Sem 6
/*
*Arduino code to sense the sensor output from weather monitoring sensors,
*------------------------
*Temperature sensor
*Humidity Sensor
*Rain Drop Sensor
*Pressure sensor
*------------------------
*These are the sensors used in second mid evaluation of BTP on 26th and 27th of April.
*/
// Short note on the project for Sem 7
/*
*Arduino code to sense and monitor the Pomagranate Crop, here weather monitorinh system is also used:
*
*Main new Sensors are :-
*------------------------
*Moisture Sensor
*------------------------
*
*Previously used sensor :-
*------------------------
*Temperature sensor
*Humidity Sensor
*Rain Drop Sensor
*Pressure sensor
*------------------------
*These are the sensors used in next evaluation of BTP in Sem 7.
*/

// Including all the libraries needed
#include <SoftwareSerial.h>
#include <SFE_BMP180.h>  // For Pressure Sensor
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

SoftwareSerial s(6,7);
DHT dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RainSensor A0     // to record the Rainfall values
#define moisture A1       // to read the moisture values of soil

// Declaring all the variables needed for this project
SFE_BMP180 pressure;
double T,P;               // Pressure sensor
int arain, drain;         // Rainfall sensor
int hum, temp;            // DHT sensor
int analog_moisture;      // Moisture sensor
int digital_moisture;

// Void setup() function 
void setup() {
  // Open serial communications and wait for port to open:
  pinMode(RainSensor,INPUT);
  pinMode(moisture,INPUT);
  Serial.begin(9600);
  s.begin(9600);
  dht.setup(2);
  
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome to BTP");
  lcd.setCursor(3,1);
  lcd.print("Project");
  
  delay(1000);

  // BMP180 Sensor
  if (pressure.begin()){
    //BMP180 init success
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BMP180 init done");
  }
  else{
    //BMP180 init fail\n\n
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BMP180 init fail");
    while(1); 
  }
}

// Reading Moisture and Scaling Up
void read_moisture() {
  analog_moisture = analogRead(moisture);
  digital_moisture = map(analog_moisture, 0, 1023, 0, 255);   // Scaling down rainfall sensor
}

// Reading Pressure Value
void readPresure(){
  char status;
 
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
 
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:      
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
         // Blank
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}

void readRain(){
  arain = analogRead(RainSensor);
  drain = map(arain, 0, 1023, 0, 255);      // Scaling down rainfall sensor
}

void readHumidity(){
  // Code to read humidity from humidity sensor
  delay(dht.getMinimumSamplingPeriod());  /* Delay of amount equal to sampling period */  

  hum = dht.getHumidity();    /* Get humidity value */
  temp = dht.getTemperature(); /* Get temperature value */
}

void upload(){
  // Sending data to NodeMCU to upload data to cloud
  String buf;
  buf += String(temp);
  buf += ";";
  buf += String(hum);
  buf += ";";
  buf += String(P*0.0295333727,4);
  buf += ";";
  buf += String(drain);
  buf += ";";
  buf += String(digital_moisture);
  s.println(buf);

  // Sending data to System to predict PH and upload to cloud
  String buf2;
  buf2 += String(temp);
  buf2 += ",";
  buf2 += String(hum);
  buf2 += ",";
  buf2 += String(drain);
  buf2 += ",";
  Serial.println(buf2);
}

// Void loop() function
void loop() {
  // Reading All Sensors Outputs
  readPresure();
  delay(500);
  readHumidity();
  delay(500);
  readRain();
  delay(500);
  read_moisture();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.print(temp);               // In degree Celcius (*C)
  
  lcd.setCursor(9,0);
  lcd.print("Hum:");
  lcd.print(hum);                // In percentage (%) Relative humidity

  lcd.setCursor(4,1);
  lcd.print("Rain:");
  lcd.print(drain);              // Low output Raining, High Output Sunlight (measures percentage)

  delay(2000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Pressure:");
  lcd.print(P*0.0295333727);     // 
  
  lcd.setCursor(0,1);
  lcd.print("Moisture:");
  lcd.print(digital_moisture);   // Low output Wet, High Output Dry (measures percentage)
  
  // Uploading to File Using NodeMCU
  upload();
  delay(500); 
}
