// This code will integrate the Peltier element – Mosfet circtuit and the thermistor circuit to control the temperature of the landing platform. 
//In this code the target temperature (VARIABLE: target_temp; default 35°C) can be set. 

//A thermistor is a resistor whose resistance varies with temperature. If the resistance of a thermistor is known the temperature can be calculated by using the Steinhart-Hart equation: 1/T = A + B(lnR) + C(lnR)3; 
//where T is temperature in Kelvin (K), R is the resistance in Ohms (Ω) and A, B and C are Steinhart-Hart coefficients. These coefficients will vary between different types of thermistors. For the thermistors used in 
//this setup (WM222C), A = 1.4713426E-03; B = 2.3762394E-04; C = 1.0503423E-07. 

//Using a voltage divider, the Arduino can calculate the resistance of the thermistor and thus the temperature. 
// We can calculate the resistance on the thermistor using the equation Rthermistor = Rreference * (Vin/Vout -1), 
//where Rreference = 1KΩ, Vin = 5V (from the 5V Arduino pin), and Vout is the voltage measured by the Arduino on the analog pin A0.


//The default reference resistance (VARIABLE: SERIESRESISTOR) is 1000 Ohm. If a different reference resistance is used for the thermistor circuit, the new resistance should be updated on this code. 
//In this code the Arduino measures the voltage Vout (VARIABLE: reading), calculates the resistance of the thermistor (VARIABLE: resistance), then calculates the temperature (VARIABLE: temperature) using the Steinhart-Hart equation and converts it to °C. 
//If the temperature is below the target temperature the Arduino will send “HIGH” pulses of 300μs (VARIABLE: pulse_duration) with an interval of 5000μs (VARIABLE: inter_pulse) between pulses.// 
//In our version of the setup this pulse length keeps the temperature constant with very little fluctuation. These values can be changed in case a stable temperature cannot be achieved. 
//Alternatively, a code that uses a PID control can be uploaded to the Arduino instead. We recommend using the PID code only if a stable temperature cannot be achieved with this code. 
//Additionally this code sends an output of 5v (HIGH) out of 4 pins (Pin_LED_one-four) to control the 12 LED light of the setup

// IMPORTANT: check Port of this Arduino and update on python code: Temperature_arduino.py

const int PIN_PELTIER = 3; // define pin to control peltier
const int Pin_LED_one = 8; // define pin to control LED circuit 1
const int Pin_LED_two = 9; // define pin to control LED circuit 2
const int Pin_LED_three = 10; // define pin to control LED circuit 3
const int Pin_LED_four = 11; // define pin to control LED circuit 4

// the ohmage of the reference resistor in the thermistor circuit
#define SERIESRESISTOR 1000

// What pin to connect the sensor to
#define THERMISTORPIN A0

//Define target temperature
#define target_temp 35

//Define pulse duration
#define pulse_duration 300

// Define inter pulse time
#define inter_pulse 5



void setup() {
  // define outpit pins
  pinMode(PIN_PELTIER, OUTPUT);
  pinMode (Pin_LED_one, OUTPUT);
  pinMode (Pin_LED_two, OUTPUT);
  pinMode (Pin_LED_three, OUTPUT);
  pinMode (Pin_LED_four, OUTPUT);

  // define input pin
  pinMode(THERMISTORPIN, INPUT);
  
  digitalWrite(PIN_PELTIER, LOW); // set satarting condition of Pin_peltier to LOW (OFF)

  // Turn on LED pins 
  digitalWrite (Pin_LED_one, HIGH);
  digitalWrite (Pin_LED_two, HIGH);
  digitalWrite (Pin_LED_three, HIGH);
  digitalWrite (Pin_LED_four, HIGH);

  Serial.begin(9600); // Start serial communication to send temeprature information to Python (code Temperature_arduino.py)
}

int g_PrintCycle = 0;

void loop() {

  float reading;
  float resistance;
  float temperature;

  reading = analogRead(THERMISTORPIN); // read voltage (Vout) on Analog pin 


  // convert the value to resistance
  resistance = SERIESRESISTOR / ((1023 / reading)  - 1) ;  // 1K / (1023/ADC - 1) 


  // convert the resistance value to a temperature value in °C
  // Steinhart-Hart Equation= 1/T= A + B*lnR + C(lnR)^3   NOTE= T is in Kelvins
  
 // define coefficients IMPORTANT: these coefficients vary between thermistors. Update the values for the thermistor being used
  float A = 0.0014713426; // define coefficient A of Steinhart-Hart Equation
  float B = 0.00023762394; // define coefficient B of Steinhart-Hart Equation
  float C = 0.00000010503423; // define coefficient C of Steinhart-Hart Equation


  temperature = 1 / (A + (B * log(resistance)) + (C * (pow(log(resistance), 3)))); // Calculate temperature from resistance using Steinhart-Hart Equation
  temperature = temperature - 273.15; // convert from K to °C

  if (temperature < -0) { // if there is miscommunication between the Arduino and the thermistor, and the reported temperature value is too low, keep Peltier off to avoid overheating

    digitalWrite(PIN_PELTIER, LOW);
    delay(5);

  }

  else if (temperature < target_temp) { // if the measured temperature is below the target temperature send a pulse of pulse_duration in length with an inteval inter_pulse

    digitalWrite(PIN_PELTIER, LOW);
    delay(inter_pulse);  // time between pulses
    digitalWrite(PIN_PELTIER, HIGH); // output 5V
    delayMicroseconds(pulse_duration);// length of pulse
    
  }
  else if (temperature >= target_temp) { // if the measured temperature is above the target temperature leave Peltier off
    digitalWrite(PIN_PELTIER, LOW);
    delay(5);
  }




  if (g_PrintCycle == 0) { // every 10 cycles send the temperature value to Python via serial communication
    Serial.println(temperature);
  }
  g_PrintCycle = (g_PrintCycle + 1) % 10;



}
