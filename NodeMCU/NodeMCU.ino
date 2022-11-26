/*======================================================================================*/
                                /*NODEMCU code*/
/*======================================================================================*/

/*Nodemcu code to connect with ThingSpeak cloud*/
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

//----------- Enter you Wi-Fi Details---------//
char ssid[] = "Abbas-mi";  // your network SSID (name)
char pass[] = "12345678";  // your network password
//-------------------------------------------//

//----------- Channel Details -------------//
#define channelID 1705736
const char mqttUserName[] = "LA0XFSUXLTcYDigpBioXNDE"; 
const char clientID[] = "LA0XFSUXLTcYDigpBioXNDE";
const char mqttPass[] = "7KGd22Yg0kz13Oh3oT0BtAbi";
// ----------------------------------------//

#define mqttPort 1883
WiFiClient client;

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );
SoftwareSerial s(D5, D6);                     // RX and TX pins for Weather System

double pressure;        // weather system
int hum, temp;          // weather system 
int rain;               // weather system
String Data[5];         // weather system
int moisture;           // crop system

unsigned long timeChange;
const unsigned long interval = 1800000;

void connectWifi()
{
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}

void setup(){
   Serial.begin(9600);
   s.begin(9600);
   
   connectWifi();
  
   // Configure the MQTT client
   mqttClient.setServer( server, mqttPort ); 
  
   // Set the MQTT message handler function.
   mqttClient.setCallback( mqttSubscriptionCallback );
  
   // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
   mqttClient.setBufferSize( 2048 );

   timeChange = millis();
}

// Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ){
  //String myTopic = "channels/"+String( subChannelID )+"/subscribe/fields/field1";
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  mqttClient.subscribe(myTopic.c_str());
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

void get_Value(){

  if(s.available()>0){
    String string = s.readStringUntil('\r\n');

    // Convert from String Object to String.
    char buf[50];
    string.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    int i = 0;
    while ((str = strtok_r(p, ";", &p)) != NULL){ // delimiter is the semicolon
      Data[i] = str;
      i++;
    }
  }

  temp = Data[0].toInt();
  hum = Data[1].toInt();
  pressure = Data[2].toInt();
  rain = Data[3].toInt();
  moisture = Data[4].toInt();

  Serial.print("Temperature :: ");
  Serial.println(temp);
  Serial.print("Humidity :: ");
  Serial.println(hum);
  Serial.print("Pressure :: ");
  Serial.println(pressure);
  Serial.print("Rainfall :: ");
  Serial.println(rain);
  Serial.print("Moisture level :: ");
  Serial.println(moisture);
  Serial.println("/**********************/");

}

void loop(){

  get_Value();
  
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }
  
  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
     //mqttSubscribe( channelID );
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 
  
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  if(millis() - timeChange >= interval){
    mqttPublish( channelID, (String("field1=")+String(temp)+"&"+String("field2=")+String(hum)+"&"+String("field3=")+String(pressure)+"&"+String("field4=")+String(rain)+"&"+String("field5=")+String(moisture)) );
    timeChange = millis();
  }
  
  delay(2000);
}
