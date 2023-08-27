#include <LiquidCrystal.h>

const int rs = 13, en = 8, d4 = 7, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define button_U    1
#define button_E    0
#define ONBOARD_LED 13
#define DEFAULT_DELAY 300

int LRemotor1 = 5;
int LFomotor1 = 6;
int RFomotor2 = 9;
int RRemotor2 = 10;
int steps=0;

int sensor[6] = {A5, A4, A3, A2, A1, A0};
int sensorReading[6] = { 0 };
float activeSensor = 0; 
float totalSensor = 0;
float avgSensor = 0; 
int weights[6] = {-10, -5, -1, 1, 5, 10};

float Kp = 0;   // Max deviation = 10 - 0 = 10 ||  255/10 = 25
float Ki = 0.00015;
float Kd = 2;

float error = 0;
float previousError = 0;
float totalError = 0;

float power = 0;

int PWM_Left;
int PWM_Right;

int MAX_SPEED = 30;

char buttonPressed = '0';

byte menuLevel = 0;     // Level 0: no menu display, display anything you like
                        // Level 1: display main menu
                        // Level 2: display sub menu
                        // Level 3: display sub menu of sub menu

byte menu = 1;
byte sub = 1;

unsigned long var_1 = 0;
unsigned long var_2 = 0;
unsigned long var_3 = 0;

bool LED_STATE = false;

bool currState_U = HIGH;
bool currState_E = HIGH;
           
bool prevState_U = HIGH; 
bool prevState_E = HIGH; 

unsigned long prevTime_U = 0;
unsigned long prevTime_E = 0;

unsigned long waitTime_U = 50;
unsigned long waitTime_E = 50;

byte charUp[8] = {
        B00100,
        B01110,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
};
byte charDown[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B01110,
        B00100
};
byte charUpDown[8] = {
        B00100,
        B01110,
        B11111,
        B00000,
        B00000,
        B11111,
        B01110,
        B00100
};


void setup() {
  for(int i=0; i<6; i++) {
    pinMode(sensor[i], INPUT);
  }
  
  lcd.begin(16,2);
  lcd.createChar(0, charUp);
  lcd.createChar(1, charDown);
  lcd.createChar(2, charUpDown);

  pinMode(button_U, INPUT_PULLUP);
  pinMode(button_E, INPUT_PULLUP);

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LED_STATE);

  pinMode(LRemotor1, OUTPUT);
  pinMode(LFomotor1, OUTPUT);
  pinMode(RFomotor2, OUTPUT);
  pinMode(RRemotor2, OUTPUT);
  
  Serial.begin(9600);

  updateLevel_0();
}

void loop() {
  checkButton();
  lineFollow();
 
}

void checkButton() {

  // Button Debouncing
  bool currRead_U = digitalRead(button_U);
  bool currRead_E = digitalRead(button_E);

  if (currRead_U != prevState_U) {
    prevTime_U = millis();
  }
  if (currRead_E != prevState_E) {
    prevTime_E = millis();
  }

 else buttonPressed = '0';
  if ((millis() - prevTime_U) > waitTime_U) {
    if (currRead_U != currState_U) {
      currState_U = currRead_U;
      if (currState_U == LOW) {
        buttonPressed = 'U';
      } else {
        //buttonPressed = '0';
      }
    }
  } else buttonPressed = '0';
  if ((millis() - prevTime_E) > waitTime_E) {
    if (currRead_E != currState_E) {
      currState_E = currRead_E;
      if (currState_E == LOW) {
        buttonPressed = 'E';
      } 
    }
  } else buttonPressed = '0';

  prevState_U = currRead_U;
  prevState_E = currRead_E;

  processButton(buttonPressed);

}

void processButton(char buttonPressed) {

  switch(menuLevel) {
    case 0:                     // Level 0, home screen
      switch ( buttonPressed ) {
        case 'E':               // Enter
          menu = 1;
          menuLevel = 1;        // go to main menu
          updateLevel_1();      // show main menu
          delay(DEFAULT_DELAY);
          break;
        case 'U': // Up
          break;
        default:
          break;
      }
      break;
    case 1:                     // Level 1, main menu
      switch ( buttonPressed ) {
        case 'E':               // Enter
          sub = 1;
          menuLevel = 2;        // go to sub menu
          updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'U':               // Up
          menu++;
          updateLevel_1();      // show main menu
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      } 
      break;
    case 2:                     // Level 2, sub menu
      switch ( buttonPressed ) {
        case 'E':               // Enter
          if (sub == 1) {
            menuLevel = 3;
            steps=1;
            updateLevel_4();
          } if (sub == 2) {       // Jump to sub menu of sub menu only when edit is necessary
            menuLevel = 3;        // go to sub menu of sub menu
            steps=2;
            updateLevel_3();      // show sub menu of sub menu
          } 
          delay(DEFAULT_DELAY);
          break;
        case 'U':               // Up
          sub++;
          updateLevel_2();
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      } 
      break;
    case 3:                     // Level 3, sub menu of sub menu
      switch ( buttonPressed ) {
        case 'E':               // Enter
          menuLevel = 2;        // go back to level 2
          updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'U':               // Up
          if (sub == 1 && steps==1) {       // edit value
            if        (menu == 1) {
              if (MAX_SPEED < 180) {  // 1 hour max
                MAX_SPEED = MAX_SPEED + 10;
              } else {
                MAX_SPEED = 30;
              }
            } else if (menu == 2) {       
              if (MAX_SPEED < 180) {  // 1 hour max
                MAX_SPEED = MAX_SPEED + 20;
              } else {
                MAX_SPEED = 30;
              }
            } else if (menu == 3) {
              if (MAX_SPEED < 180) {  // 1 hour max  
                MAX_SPEED = MAX_SPEED + 30;
              } else {
                MAX_SPEED = 30;
              }
            }
          }          
          updateLevel_4();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        default:  
          break;
      }
    case 4:                     // Level 3, sub menu of sub menu
      switch ( buttonPressed ) {
        case 'E':               // Enter
          menuLevel = 2;        // go back to level 2
          updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'U':               // Up          
          if (sub == 2 && steps==2) {       // edit value
            if        (menu == 1) {
              if (Kp < 30) {  // 1 hour max
                Kp = Kp + 1;
              } else {
                Kp = 30;
              }
            } else if (menu == 2) {       
              if (Kp < 30) {  // 1 hour max
                Kp = Kp + 4;
              } else {
                Kp = 30;
              }
            } else if (menu == 3) {
              if (Kp < 30) {  // 1 hour max 
                Kp = Kp + 7; 
              } else {
                Kp = 30;
              }
            }
          }
          updateLevel_3();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        default:  
          break;
      } 
      break;
    default:
      break;
  }
  
}

void updateLevel_0() {
  lcd.clear();
  lcd.println("    Kelompok 3  ");
  lcd.setCursor(0,1);
  lcd.println("  Line Tracking  ");
}

void updateLevel_1 () {
  
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Set up 1: ");
      lcd.setCursor(0, 1);
      lcd.print(" Set up 2: ");
      lcd.setCursor(15,1);
      lcd.write((byte)1);     // down arrow
      break;
    case 2:
      lcd.clear();
      lcd.print(" Set up 1: ");
      lcd.setCursor(0, 1);
      lcd.print(">Set up 2: ");
      lcd.setCursor(15,1);
      lcd.write((byte)2);     // up and down arrow
      break;
    case 3:
      lcd.clear();
      lcd.print(">Set up 3: ");
      lcd.setCursor(0, 1);
      lcd.print("             ");
      lcd.setCursor(15,1);
      lcd.write((byte)0);     // up arrow
      break;
    case 4:
      menu = 0;
      break;
  }
  
}

void updateLevel_2 () {
  switch (menu) {
    case 0:
      break;
    case 1:                                 // Set line follower 1
      switch (sub) {
        case 0:
          break;
        case 1:
          lcd.clear();
          lcd.print(" Set up 1 ");
          lcd.setCursor(0, 1);
          lcd.print("  Speed = ");
          lcd.print(MAX_SPEED);
          lcd.setCursor(15,1);
          lcd.write((byte)1);     // down arrow
          break;
        case 2:
          lcd.clear();
          lcd.print(" Set up 1 ");
          lcd.setCursor(0, 1);
          lcd.print("  Set P = ");
          lcd.print(Kp);
          lcd.setCursor(15,1);
          lcd.write((byte)2);     // up and down arrow
          break;
        default:
          break;
      }
      break;
    case 2:                               
      switch (sub) {
        case 0:
          break;
        case 1:
          lcd.clear();
          lcd.print(" Set up 2 ");
          lcd.setCursor(0, 1);
          lcd.print("  Speed = ");
          lcd.print(MAX_SPEED);
          lcd.setCursor(15,1);
          lcd.write((byte)1);     // down arrow
          break;
        case 2:
          lcd.clear();
          lcd.print(" Set up 2:");
          lcd.setCursor(0, 1);
          lcd.print("  Set P = ");
          lcd.print(Kp);
          lcd.setCursor(15,1);
          lcd.write((byte)2);     // up and down arrow
          break;
        default:
          break;
      }
      break;
    case 3:                               // Relay 3
      switch (sub) {
        case 0:
          break;
        case 1:
          lcd.clear();
          lcd.print(" Set Up 3:");
          lcd.setCursor(0, 1);
          lcd.print("  Speed = ");
          lcd.print(MAX_SPEED);
          lcd.setCursor(15,1);
          lcd.write((byte)1);     // down arrow
          break;
        case 2:
          lcd.clear();
          lcd.print(" Set up 3:");
          lcd.setCursor(0, 1);
          lcd.print("  Set P = ");
          lcd.print(Kp);
          lcd.setCursor(15,1);
          lcd.write((byte)2);     // up and down arrow
          break;
        default:
          break;
      }
      break;
    case 4:
      sub = 1;
      break;
  }
}

void updateLevel_4 () {
  switch (menu) {
    case 0:
      break;
    case 1:
      lcd.clear();
      lcd.print(" Set Speed ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(MAX_SPEED);
      break;
    case 2:
      lcd.clear();
      lcd.print(" Set Speed ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(MAX_SPEED);
      break;
    case 3:
      lcd.clear();
      lcd.print(" Set Speed ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(MAX_SPEED);
      break;
    case 4:
      sub = 0;
      break;
  }
}

void updateLevel_3 () {
  switch (menu) {
    case 0:
      break;
    case 1:
      lcd.clear();
      lcd.print(" Set Proposional ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(Kp);
      break;
    case 2:
      lcd.clear();
      lcd.print(" Set Proposional ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(Kp);
      break;
    case 3:
      lcd.clear();
      lcd.print(" Set Proposional ");
      lcd.setCursor(0, 1);
      lcd.print("  Nilai = ");
      lcd.print(Kp);
      break;
    case 4:
      sub = 0;
      break;
  }
}
// program line follower

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
//    
//    delay(200);
}


void go(int speedLeft, int speedRight) {
  if (speedLeft > 0) {
    analogWrite(LFomotor1, speedLeft);
    analogWrite(LRemotor1, 0);
  } 
  else {
    analogWrite(LFomotor1, 0);
    analogWrite(LRemotor1, -speedLeft);
  }
 
  if (speedRight > 0) {
    analogWrite(RFomotor2, speedRight);
    analogWrite(RRemotor2, 0);
  }else {
    analogWrite(RFomotor2, 0);
    analogWrite(RRemotor2, -speedRight);
  }
}
void readSensor() {
  for(int i=0; i<6; i++) 
    {
       if(analogRead(sensor[i])>790){
         sensorReading[i]=1;
       }
    }

    Serial.print("sensor: ");
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
//    Serial.print("avgSensor=");
//    Serial.println(avgSensor);
    
    activeSensor = 0; totalSensor = 0;
    for(int i=0; i<6; i++) 
    {
      sensorReading[i]=0;
    }
}
