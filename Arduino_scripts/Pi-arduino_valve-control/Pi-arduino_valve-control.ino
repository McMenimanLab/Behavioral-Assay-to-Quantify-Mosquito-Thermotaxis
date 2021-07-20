// This code controls the solenoid valve for odor stimulation. It receives a signal from the Raspberry Pi GPIO pin, 
//waits for a defined time (pre) and switches the valve on for a defined time (stim_on). Every 500ms the output
// pin status is sent over serial communication to Python (see code pi_signal_to_arduino.py).

int stimulus = 3; // define output pin to control valve
int trigger = 4; // define input pin to receive trigger signal

// choose how many seconds before swtiching stimulus ON and multiply by 2
#define pre 30*2

// choose how many seconds of stimulus on and multiply by 2

#define stim_on 60*2



void setup() {

  pinMode(stimulus, OUTPUT); // set stimulus pin as OUTPUT
  pinMode(trigger, INPUT); // set trigge pin as INPUT

  Serial.begin(9600); // begin serial communication to send stimulus satuts to Python
  
  digitalWrite(stimulus, LOW); // set the initual stimulus status as LOW (OFF)
  
  // put your setup code here, to run once:

}

int trigger_status = 0; // define initial trigger status
int stim_status = 0; // define initial stimulus status

void loop() {
  // put your main code here, to run repeatedly:

  trigger_status = digitalRead(trigger); // read trigger pin

    if (trigger_status == HIGH) { // if trigger is on

      delay(500); // wait 500ms for camera to start recording (see code video_trigger.py)

      
      for (int i = 0; i < pre; i++){ //send stimulus status to Python every 500ms during pre-stimulus time (pre)

        stim_status = digitalRead(stimulus);
        Serial.println(stim_status);
        delay(500);
        
      }

      
      
      digitalWrite(stimulus, HIGH); //after pre-stimulus time turn open valve

      
      for (int i = 0; i < stim_on; i++){ // send stimulus status to Python every 500ms during stimulus on time (stim_on)
      
        stim_status = digitalRead(stimulus);
        Serial.println(stim_status);
        delay(500);
        
      }
      
      digitalWrite(stimulus, LOW); // turn off stimulus
    }
      
      else if (trigger_status == LOW); { // if the trigger status is LOW keep valve off and report stimulus status

      stim_status = digitalRead(stimulus);
      Serial.println(stim_status);
        }
}
   
  
