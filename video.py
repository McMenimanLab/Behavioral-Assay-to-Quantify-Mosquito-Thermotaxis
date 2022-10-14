# Run this code to start recording a movie at a specific time (set start time in line 19)
# Define the duration of the recording in line 11 

#import required packages

import picamera
import time
import datetime


experiment_duration = 3600 # DEFINE EXPERIMENT DURATION IN SECONDS

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
            now=datetime.datetime.now() #get date
            date=now.strftime("%m-%d-%Y")
            current_time=now.strftime("%H-%M-%S")
            camera.start_recording('/home/pi/Desktop/Data/Movie/'+date+'_'+current_time+'.h264') # start recording and save video in .h264 format
            camera.wait_recording(experiment_duration) # set recording length (default 1 hour)
            camera.stop_recording() # stop recording
            break







