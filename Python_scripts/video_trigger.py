# This code will send out a TTL pulse of 0.5s to the Arduino 2 to start stimulus valve protocol (see code Pi-arduino_valve-control)
# and start recording a video.
# Define the duration of the recording in line 20 and the start time in line 22 

#import required packages

import picamera
import time
import datetime
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(10, GPIO.OUT) # define Raspberry Pi GPIO pin 10 as output



experiment_duration = 120 # DEFINE EXPERIMENT DURATION IN SECONDS

while True: #start an infite loop and wait for the set time (line 25) to start the recording
    
    hour = float(time.strftime('%H'))
    minute = float(time.strftime('%M'))
    second = float(time.strftime('%S'))
    
    if hour ==21 and minute == 59 and second == 58: # SET THE EXPERIMENT START TIME. THE CAMERA WILL TAKE APROX. 2s TO START RECORDING SO TIME MUST BE SET 2s BEFORE DESIRED START
        with picamera.PiCamera() as camera:

            camera.framerate=10 # set camera frame rate (default = 10fps)
            camera.shutter_speed=camera.exposure_speed
            camera.exposure_mode='off'
            g=camera.awb_gains
            camera.awb_mode='off'
            camera.awb_gains=g
            camera.brightness=50
    

            camera.resolution=(1080, 1080) 
            camera.zoom=(0.25, 0.27, 0.5, 0.5) # set the ROI size and location (see code run_preview.py)
            time.sleep(2) # Allow 2 seconds for the camera to adjust parameters
    
            GPIO.output(10, GPIO.HIGH) # Send TTL pusle out of GPIO pin 10
            time.sleep(0.5) # length of pulse
            GPIO.output(10, GPIO.LOW) # Set GPIO pin 10 bck to LOW
            now=datetime.datetime.now() #get date
            date=now.strftime("%m-%d-%Y")
            current_time=now.strftime("%H-%M-%S")
            camera.start_recording('/home/pi/Desktop/Protocol_data/Movie/'+date+'_'+current_time+'.h264') # start recording and save video in .h264 format
            camera.wait_recording(experiment_duration) # set recording length (default 15 minutes)
            camera.stop_recording() # stop recording
            break







