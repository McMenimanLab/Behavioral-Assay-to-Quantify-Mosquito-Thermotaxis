# This code allows you to set the camera location and zoom. If run on terminal quit script by pressing
# ctrl C

# Import required packages
from picamera import PiCamera
from time import sleep

camera = PiCamera()

camera.start_preview() # start the camera preview
camera.zoom=(0.25, 0.5, 0.3, 0.3) # set the ROI size and location

sleep(50) # determine how long to run preview for. 
camera.stop_preview()
