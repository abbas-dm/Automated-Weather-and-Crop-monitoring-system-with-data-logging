# Automated-Weather-and-Crop-monitoring-system-with-data-logging

Our weather forecast system constantly monitors the temperature, humidity, rainfall, and pressure and uploads them to the cloud. The cloud is updated in 
real-time with the app. Intelligent crop monitoring is one of the most important things to do to reduce crop losses. This helps farmers take the correct 
steps and improve production quality. Crop monitoring typically helps control various pests, weeds, or diseases in plants.

For our crop monitoring system, we took pomegranate as the test subject. Predict soil pH using a trained ML model and monitor moisture. Alerts are sent to 
farmers when monitored parameters are not in the range of threshold values. This helps farmers take necessary actions to avoid unnecessary losses.


# Code files working 
### Arduino code file - 
This is used to program Arduino Uno to collect data from all the sensor connected to it and send that data to NodeMcu using UART communication protocol as a string.

### NodeMcu code file - 
This is used to program NodeMcu to collect the string data sent by Arduino Uno and extract the values from it and to upload data to ThingSpeak platform using MQTT light-weigth 
communication protocol.

### PH_Prediction.py - 
This the python code to train the ML model on the dataset to predict the PH value and saves the trained ML model with ".h" extention using Pickle library.

### Arduino_read.py - 
This is the Algorithm which continuously reads the input attribute data from Arduino Uno using Serial USB communication and by using Pickel library reads the
saved ML model to predict the PH value based on input attributes data, It will also directly uploaded the data to ThingSpeak Cloud.
