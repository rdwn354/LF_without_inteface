/* 
 *  This is a programm for line followers
 *  this robot uses arduino uno as the microcontroller 
 *  and uses PID sistem for the main system
*/

//Set Pin Number of Motor DC
#define LRemotor1 5
#define LFomotor1 6
#define RFomotor2 9
#define RRemotor2 10
 
int sensor[6] = {A5, A4, A3, A2, A1, A0};
int sensorReading[6] = { 0 };
float activeSensor = 0; 
float totalSensor = 0;
float avgSensor = 0; 
int weights[6] = {-10, -5, -1, 1, 5, 10};

//PID Adjustment
float Kp = 15; 
float Ki = 0.00015;
float Kd = 2;

float error = 0;
float previousError = 0;
float totalError = 0;

int error1;
float power = 0;

int PWM_Left;
int PWM_Right;

int MAX_SPEED = 100;

void setup() {
  pinMode(LRemotor1, OUTPUT);
  pinMode(LFomotor1, OUTPUT);
  pinMode(RFomotor2, OUTPUT);
  pinMode(RRemotor2, OUTPUT);

  for(int i=0; i<6; i++) {
    pinMode(sensor[i], INPUT);
  }
  Serial.begin(9600);
  
}

void loop() {
   lineFollow();
}

void lineFollow(void) {
   PID_program();
   go(PWM_Left,PWM_Right);
}

void PID_program()
{ 
    readSensor();
    
    previousError = error;
    error = avgSensor;
    totalError += error; 
//    Serial.print("error=");
//    Serial.println(error);
    power = (Kp*error) + (Kd*(error-previousError));// + (Ki*totalError);
    
    if( power>MAX_SPEED ) { power = MAX_SPEED; }
    if( power<-MAX_SPEED ) { power = -MAX_SPEED; }
//    Serial.print("POWER=");
//    Serial.println(power);
    if(power<0) // Turn left
    {
      PWM_Right = MAX_SPEED;// + abs(int(power));
      PWM_Left = MAX_SPEED - abs(int(power));
    }
    
    else // Turn right
    {
      PWM_Right = MAX_SPEED - int(power);
      PWM_Left = MAX_SPEED;// + abs(int(power));
    }
//    Serial.print("PWM_Right=");
//    Serial.print(PWM_Right);
//    Serial.println(" ");
//    Serial.print("PWM_Left=");
//    Serial.print(PWM_Left);
//    Serial.println(" ");
//    delay(200);
}


void go(int speedLeft, int speedRight) {
  if (speedLeft > 0){
    analogWrite(LFomotor1, speedLeft);
    analogWrite(LRemotor1, 0);
  }else{
    analogWrite(LFomotor1, 0);
    analogWrite(LRemotor1, -speedLeft);
  }
 
  if (speedRight > 0){
    analogWrite(RFomotor2, speedRight);
    analogWrite(RRemotor2, 0);
  }else {
    analogWrite(RFomotor2, 0);
    analogWrite(RRemotor2, -speedRight);
  }if(error1==2){
    analogWrite(RFomotor2, 60);
    analogWrite(RRemotor2, 0);
  }else if(error1==3){
    analogWrite(LFomotor1, 60);
    analogWrite(LRemotor1, 0);
  }else if(error1==4){
    analogWrite(RFomotor2, 0);
    analogWrite(RRemotor2, 0);
    analogWrite(LFomotor1, 0);
    analogWrite(LRemotor1, 0);
  } 
}
void readSensor() {
  for(int i=0; i<6; i++) 
    {
       if(analogRead(sensor[i])>790){
         sensorReading[i]=1;
       }
    }
    if(sensorReading[0]==1 && sensorReading[1]==1 && sensorReading[2]==1 && sensorReading[3]==1 && sensorReading[4]==1 && sensorReading[5]==0){
      int error1 = 2;}//left
    else if(sensorReading[0]==0 && sensorReading[1]==1 && sensorReading[2]==1 && sensorReading[3]==1 && sensorReading[4]==1 && sensorReading[5]==1){
      int error1 = 3;}//right
    else if(sensorReading[0]==1 && sensorReading[1]==1 && sensorReading[2]==1 && sensorReading[3]==1 && sensorReading[4]==1 && sensorReading[5]==1){
      int error1 = 4;//stop
    }
      
    Serial.print("senzori: ");
    for(int i=0; i<6; i++) 
    {
      Serial.print(sensorReading[i]);
      Serial.print(" ");
    }
    Serial.println(" ");


    for(int i=0; i<6; i++) 
    {
      if(sensorReading[i]==1) { activeSensor+=1; }
      totalSensor += sensorReading[i] * weights[i];
    }  
    avgSensor = totalSensor/activeSensor;
//    Serial.println("totalSensor   activeSensor    avgSensor");
//    Serial.print(totalSensor);
//    Serial.print("      ");
//    Serial.print(activeSensor);
//    Serial.print("      ");    
//    Serial.println(avgSensor);
//    delay(500);
    activeSensor = 0; totalSensor = 0;
    for(int i=0; i<6; i++) 
    {
      sensorReading[i]=0;
    }
}
