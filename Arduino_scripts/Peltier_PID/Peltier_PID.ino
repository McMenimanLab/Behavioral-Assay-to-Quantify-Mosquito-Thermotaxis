// This code will integrate the Peltier element – Mosfet circtuit and the thermistor circuit to control the temperature of the landing platform using a PID control. 
//In this code the target temperature (VARIABLE: target_temp; default 35°C) can be set. If a different thermistor is used, the A, B and C coefficients should be updated. 


//A thermistor is a resistor whose resistance varies with temperature. If the resistance of a thermistor is known the temperature can be calculated by using the Steinhart-Hart equation: 1/T = A + B(lnR) + C(lnR)3; 
//where T is temperature in Kelvin (K), R is the resistance in Ohms (Ω) and A, B and C are Steinhart-Hart coefficients. These coefficients will vary between different types of thermistors. For the thermistors used in 
//this setup (WM222C), A = 1.4713426E-03; B = 2.3762394E-04; C = 1.0503423E-07. 

//Using a voltage divider, the Arduino can calculate the resistance of the thermistor and thus the temperature. 
// We can calculate the resistance on the thermistor using the equation Rthermistor = Rreference * (Vin/Vout -1), 
//where Rreference = 1KΩ, Vin = 5V (from the 5V Arduino pin), and Vout is the voltage measured by the Arduino on the analog pin A0.


//The default reference resistance (VARIABLE: SERIESRESISTOR) is 1000 Ohm. If a different reference resistance is used for the thermistor circuit, the new resistance should be updated on this code. 
//In this code the Arduino measures the voltage Vout (VARIABLE: reading), calculates the resistance of the thermistor (VARIABLE: resistance), then calculates the temperature (VARIABLE: temperature) using the Steinhart-Hart equation and converts it to °C. 

//This code uses a PID control for temeprature control. Use if a constant temperature cannot be achieved with code Thermistor_Peltier_control.ino.
//Ajust kp, ki and kd until a stable temperature is achieved. 
//Based on the measured temperature and the target temperature a PID_value will be calculated and used as an output to heat the Peltier element

//Additionally this code sends an output of 5v (HIGH) out of 4 pins (Pin_LED_one-four) to control the 12 LED light of the setup




const int PIN_PELTIER = 3; // define pin to control peltier
const int Pin_one=8; // define pin to control LED circuit 1
const int Pin_two=9; // define pin to control LED circuit 2
const int Pin_three=10; // define pin to control LED circuit 3
const int Pin_four=11; // define pin to control LED circuit 4

// the ohmage of the 'other' resistor in the thermistor circuit
#define SERIESRESISTOR 1000

// What pin to connect the sensor to
#define THERMISTORPIN A0

//Define target temperature
#define target_temp 35


float temperature_read = 0.0; // define initial temperature read
float PID_error = 0; // define initial PID error
float previous_error = 0; // defina initial previous error
float elapsedTime, Time, timePrev;
int PID_value = 0; // define initial PID value


//PID constants 
int kp = 2.0;   int ki = 0.7;   int kd = 1.8; // Adjust these constants until a stable temperature is achieved

int PID_p = 0;    int PID_i = 0;    int PID_d = 0;



void setup() {
   
   // define outpit pins
  pinMode(PIN_PELTIER, OUTPUT);
  pinMode (Pin_one, OUTPUT);
  pinMode (Pin_two, OUTPUT);
  pinMode (Pin_three, OUTPUT);
  pinMode (Pin_four,OUTPUT);

   // define input pin
  pinMode(THERMISTORPIN, INPUT);

  
  digitalWrite(PIN_PELTIER, LOW); // set satarting condition of Pin_peltier to LOW (OFF)

 // Turn on LED pins 
  digitalWrite (Pin_one,HIGH);
  digitalWrite (Pin_two, HIGH);
  digitalWrite (Pin_three, HIGH);
  digitalWrite (Pin_four, HIGH);

  Serial.begin(9600);// Start serial communication to send temeprature information to Python (code Temperature_arduino.py)
}

int g_PrintCycle = 0;

void loop() {

  float reading;
  float resistance;
  float temperature;

  reading = analogRead(THERMISTORPIN);// read voltage (Vout) on Analog pin 


  // convert the value to resistance
  resistance = SERIESRESISTOR / ((1023 / reading)  - 1) ;  // 1K / (1023/ADC - 1)


  // convert the resistance value to a temperature value in °C
  // Steinhart-Hart Equation= 1/T= A + B*lnR + C(lnR)^3   NOTE= T is in Kelvins

  // define coefficients IMPORTANT: these coefficients vary between thermistors. Update the values for the thermistor being used
  
  float A = 0.0014713426; // define coefficient A of Steinhart-Hart Equation
  float B = 0.00023762394; // define coefficient B of Steinhart-Hart Equation
  float C = 0.00000010503423; // define coefficient C of Steinhart-Hart Equation  


  temperature = 1 / (A + (B * log(resistance)) + (C * (pow(log(resistance), 3)))); // Calculate temperature from resistance
  temperature = temperature - 273.15; // convert from K to °C

 //Serial.println(temperature);
  
  if (temperature < -0) { // if there is miscommunication between the Arduino and the thermistor, and the reported temperature value is too low, keep Peltier off to avoid overheating

    digitalWrite(PIN_PELTIER, LOW);
    delay(5);
    
  }
  
  else if (temperature > 0) { // if the measured temperature is below the target temperature calculate PID_value and send a PWM pulse out of PIN_PELTIER
    PID_error = target_temp - temperature;
    
    //Calculate the P value
    PID_p = kp * PID_error;
    
    //Calculate the I value in a range of +-3
    if(-3 < PID_error <3)
    {
      PID_i = PID_i + (ki * PID_error);
    }

    //For derivative we need real time to calculate speed change rate
    timePrev = Time;                            // store previous time
    Time = millis();                            // store current time
    elapsedTime = (Time - timePrev) / 1000; 
    
    //Calculate D value
    PID_d = kd*((PID_error - previous_error)/elapsedTime);
   
   //PID value is the sum of P + I + D
    PID_value = PID_p + PID_i + PID_d;

    //Define PWM range between 0 and 255
    
    if(PID_value < 0)  //if the PID value is below 0 keep peltier off
    {    PID_value = 0;
    
    digitalWrite(PIN_PELTIER, LOW);
    }
    
    else if(PID_value > 255)  //if the PID value is above 255 set it to 255
    
    {    PID_value = 255;  
    analogWrite(PIN_PELTIER,PID_value);
    }
    
  //Send PWM signal to Mosfet

    else if (PID_value > 0 || PID_value <255) {

      digitalWrite(PIN_PELTIER, PID_value);
    }
    previous_error = PID_error;     //Store previous error for next loop.

    delay(300);

  }

  if (g_PrintCycle == 0) {  // every 10 cycles send the temperature value to Python via serial communication
    Serial.println(temperature);
  }
  g_PrintCycle = 0;  

}
