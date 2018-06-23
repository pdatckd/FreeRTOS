/*------------------------------------------------------
Gauge_Pot01
Arduining.com 10 July 2011
Direct driving of Miniature Stepper Motor used as an indicator
drive for dashboard instrumentation.
Hardware:
-Arduino UNO.
-Potentiometer.
-Stepper Motor VID29-05P (Can be directly driven by MCU):
    -Low current consumption: <20mA 
    -Rotation Angle:Max 315°
    -0.5°/full step    
    -Coil resistance: 280 +/- 20Ω                           
 -----------------------------------------------------*/
#include <SimpleKalmanFilter.h>
#include <Stepper.h>
#define  STEPS  300    // steps per revolution (limited to 315°)
#define  COIL3  15
#define  COIL4  14
#define  COIL1  16
#define  COIL2  10

#define  COILT3  5
#define  COILT4  6
#define  COILT1  3
#define  COILT2  4
#define  LED    9

#define Sensor A0
#define SensorT A1

// create an instance of the stepper class:
Stepper stepper(STEPS, COIL1, COIL2, COIL3, COIL4);
Stepper stepperT(STEPS, COILT1, COILT2, COILT3, COILT4);
SimpleKalmanFilter Filter_SeGauge(2, 2, 0.01);
SimpleKalmanFilter Filter_SeTemp(2, 2, 0.01);
volatile int pos = 0;                 //Position in steps(0-630)= (0°-315°)
volatile int posT = 0;                 //Position in steps(0-630)= (0°-315°)
volatile int val  = analogRead(A0);
volatile int valT = analogRead(A1);
volatile unsigned long int count1;
volatile unsigned long int count2;
volatile unsigned long int count3;


void ReadGauge(void);
void LEDWarning(void);
void ControlStepper(void);

void setup(){
  stepper.setSpeed(50);    // set the motor speed to 30 RPM (360 PPS aprox.).
  stepperT.setSpeed(50);
  Serial.begin(9600);    //for debuging.
  stepper.step(-180);      // Đưa vị trí motor về vị trí nhỏ nhất.
  stepperT.step(-180);
  delay(1000);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
}
void loop(){
  if ((millis() - count1) >= 50){
    ReadGauge();
    ReadTemp();
    count1 = millis();
  };
  if ((millis() - count2) >= 100){
    LEDWarning();
    count2 = millis();
  };
  ControlStepper();
  ControlStepperTemp();
  }
void ReadGauge(void){
    val = analogRead(A0);
    if(val >= 5){
      val = map(val,350,790,60,0);
    };
    val = constrain(val,0,60);
    val = Filter_SeGauge.updateEstimate(val);
    val = Filter_SeGauge.updateEstimate(val);
    val = Filter_SeGauge.updateEstimate(val);
}
void ReadTemp(void){
    valT   = analogRead(A1);
    if (valT >= 300){
    valT   = map(valT,300,600,30,0);
    valT   = constrain(valT,0,30);
    }
    else if ((valT < 300)&&(valT >= 100)){
    valT   = map(valT,100,300,40,30);  
    valT   = constrain(valT,30,40);
    }
    else if((valT < 100)&&(valT >= 5)){
    valT   = map(valT,10,100,60,40);  
    valT   = constrain(valT,40,60);
    }
    valT   = Filter_SeTemp.updateEstimate(valT);
    valT   = Filter_SeTemp.updateEstimate(valT);
    valT   = Filter_SeTemp.updateEstimate(valT);
    Serial.println(valT);
}
void LEDWarning(void){
    if (val <= 12){
       digitalWrite(LED,HIGH);
    }
    else{
      digitalWrite(LED,LOW);
    };
}
void ControlStepper(void){
    if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          stepper.step(1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          stepper.step(-1);       // move one step to the right.
          pos--;
          }
      }
}
void ControlStepperTemp(void){
    if(abs(valT - posT)> 2){         //if diference is greater than 2 steps.
      if((valT - posT)> 0){
          stepperT.step(1);      // move one step to the left.
          posT++;
          }
      if((valT - posT)< 0){
          stepperT.step(-1);       // move one step to the right.
          posT--;
          }
      }
}

