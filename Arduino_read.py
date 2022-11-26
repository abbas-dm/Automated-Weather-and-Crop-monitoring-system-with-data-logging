# Using trained ML model to predict PH values based on temp, hum and rainfall values.

import pickle
import time
import serial,sys,glob

import urllib.request

def port_search():
    if sys.platform.startswith('win'): # Windows
        ports = ['COM{0:1.0f}'.format(ii) for ii in range(1,256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'): # MAC
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Machine Not pyserial Compatible')

    arduinos = []
    for port in ports: # loop through to determine if accessible
        if len(port.split('Bluetooth'))>1:
            continue
        try:
            ser = serial.Serial(port)
            ser.close()
            arduinos.append(port) # if we can open it, consider it an arduino
        except (OSError, serial.SerialException):
            pass
    return arduinos

arduino_ports = port_search()
ser = serial.Serial(arduino_ports[0], baudrate=9600) # match baud on Arduino
ser.flush() # clear the port

# Load Model from a file
with open('knn.h', 'rb') as f:
    model = pickle.load(f)

while(1):
    line = ser.readline()
    data = line.decode()
    data = data.split(',')
    data.remove('\r\n')

    for i in range(0, len(data)):
        data[i] = int(data[i])
    #print(data)

    new_data = []
    new_data.append(data)
    #print(new_data)

    ph = model.predict(new_data)

    val=ph[0]
    URl='https://api.thingspeak.com/update?api_key='
    KEY='75UYZN8UX4QOVCRO'
    HEADER='&field6={}'.format(val)
    NEW_URL = URl+KEY+HEADER
    print(NEW_URL)
    data=urllib.request.urlopen(NEW_URL)

    print(data)
    time.sleep(1800)
