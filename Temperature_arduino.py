# This code allows Python to communicate with Arduino 1 via serial communication and obtain
# the temperature from the thermistor, report it in the terminal, and store it in a .csv file. Make sure the correct
# port is selected on line 20. The port may vary and can be checked on the Arduino code: 
# "Thermistor_Peltier_control" in the tools > Port tab. 

# import required packages
import serial
import time
import csv
import datetime
import os

now=datetime.datetime.now() #get date
date=now.strftime("%m-%d-%Y") 

os.chdir("pi/home/Desktop/Data/Temperature") #change directory to folder where you want data
                                                        #to be stored


ser = serial.Serial('/dev/cu.usbmodem144101',9600,timeout=1) # start serial communication. Make sure the port is the correct
                                        # one to communicate with Arduino 1
ser.flushInput()

now=datetime.datetime.now()
current_time=now.strftime("%H-%M-%S")

with open("temperature_data_"+date+"_"+current_time+".csv", "a") as f: #create .csv file to write data onto
    writer=csv.writer(f, delimiter=",")   
    
    while True: # start an infinite loop
        try:
            ser_bytes = ser.readline() # read data from Arduino
            print(ser_bytes) # show temperature data
            ser_bytes=float(ser_bytes)
            current_time=time.strftime('%H %M %S') # obtain time
            writer.writerow([current_time, ser_bytes]) #write data time and temperature on csv file
            
            
        except Exception as e:# if an error in communication occurs print ERROR and continue loop
            print("ERROR")
            


