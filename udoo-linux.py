import serial
import time
import threading
import paho.mqtt.client as mqtt
import json
import uuid
import sys, errno
from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE,SIG_DFL) 

macAddress = hex(uuid.getnode())[2:-1]
macAddress = format(long(macAddress, 16),'012x')

def pub(line):
 msg = json.JSONEncoder().encode({"d":{"id":line}})
 print msg
 mqttc.publish(topic, payload=msg, qos=0, retain=False)
 print "message published"

#Set the variables for connecting to the iot service
broker = ""
topic = "iot-2/evt/status/fmt/json"
username = "use-token-auth"
password = "ZxM(k5LVpM_tZ)EM+?" #auth-token
organization = "qrrdko" #org_id
deviceType = "Ponto"

topic = "iot-2/evt/status/fmt/json"


print("MAC address: " + macAddress)


#Creating the client connection
#Set clientID and broker
clientID = "d:" + organization + ":" + deviceType + ":" + macAddress
broker = organization + ".messaging.internetofthings.ibmcloud.com"
mqttc = mqtt.Client(clientID)

#Set authentication values, if connecting to registered service
if username is not "":
 mqttc.username_pw_set(username, password=password)

mqttc.connect(host=broker, port=1883, keepalive=60)

class SerialPortManager:
    
    def __init__(self):
        self.port = serial.Serial('/dev/ttyMCC',115200,timeout=1)

    def write(self, message):
        print("Writing data to serial port: " + message)
        self.port.write(message)
        self.port.flushOutput()

    def readx(self):
        return self.port.readline()

    def close(self):
        return self.port.close()

runSerialThread = True
serialMgr = SerialPortManager()

def read_from_port(serial_port):
    while runSerialThread:
        line = serial_port.readx()
        if line:
            print("Read from serial port: " + line)
            pub(line)
    serial_port.close()


serialthread = threading.Thread(target=read_from_port, args=(serialMgr,))
serialthread.daemon = True
serialthread.start()
print("Serial thread started. Press Ctrl+C to close it.")

while True:
    try:
        time.sleep(1)
    except IOError as e:
      	if e.errno == errno.EPIPE:
        	mqttc.disconnect()
		mqttc.connect(host=broker, port=1883, keepalive=60)

