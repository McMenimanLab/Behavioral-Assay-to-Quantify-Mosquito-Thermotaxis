# This code allows Python to communicate with Arduino 2 via serial communication and obtain
# the stimulus status (0 stimulus OFF, 1 stimulus ON) and store it in a .csv file. Make sure the correct
# port is selected on line 21. The port may vary and can be checked on the Arduino code: 
# "Pi-arduino_valve-control" in the tools > Port tab. 

# import required packages
import serial
import time
import os
import csv
import datetime

now=datetime.datetime.now() #get date and time
current_time=now.strftime("%H-%M-%S")
date=now.strftime("%m-%d-%Y")

os.chdir("/home/pi/Desktop/Protocol_data/Stimulus") #change directory to folder where you want data
                                                    # to be stored

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1) # start serial communication. Make sure the port is the correct
                                                        # one to communicate with Arduino 2
    ser.flush()

    while True: # run an infinite loop
        
        with open ("stimulus_data_"+date+"_"+current_time+".csv", "a") as f: #create .csv file to write data onto
            writer=csv.writer(f, delimiter=",")   
      
            while True:
    
                try:
                    ser_bytes = float(ser.readline()) # read data from Arduino 2
                    print(ser_bytes) # print valve status (0 = OFF, 1 = ON)
                    current_time=time.strftime('%H %M %S')
                    writer.writerow([current_time, ser_bytes]) #write data time and stimulus status on csv file

                except Exception as e: # if an error in communication occurs print ERROR and continue loop
                    print("ERROR")# 
