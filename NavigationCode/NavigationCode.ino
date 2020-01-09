#include "Enes100.h"

int right1 = 0;
int right2 = 1;
int rightPWM = 3;
int left1 = 8;
int left2 = 7;
int leftPWM = 6;
int trigPin1 = 2;
int echoPin1 = 4;
int trigPin2 = 10;
int echoPin2 = 9;
int talkToSlave = 13;
int recieveFromSlave = 5;
double duration1;
int distance1;
double duration2;
int distance2;
//mission actuation
int data[16];
double delayTime = 1000 / 8;
int dataIn = 5;
int dataGot = 0;
int theNumber = 0;
int activationCode[] = {0, 0, 0, 0, 0, 0, 0, 0, 1};
int letterCode[7];

void setup() {
  // put your setup code here, to run once:
  delay(5000);

  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(talkToSlave, OUTPUT);
  pinMode(recieveFromSlave, INPUT);
  pinMode(dataIn, INPUT);
  Enes100.begin("No Cap", DATA, 8, 12, 11);

  Enes100.updateLocation();
  digitalWrite(talkToSlave, LOW);


}

int readDistanceSensor1() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);
  // Calculating the distance
  distance1 = (duration1 * 0.034 / 2);
  // Prints the distance on the Serial Monitor
  //    Enes100.print("Distance: ");
  //    Enes100.println(distance);
  return distance1;
}

int readDistanceSensor2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance2 = (duration2 * 0.034 / 2);
  // Prints the distance on the Serial Monitor
  //    Enes100.print("Distance: ");
  //    Enes100.println(distance);
  return distance2;
}

void setForward() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}

void setBackward() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}

void setRight() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
}

void setLeft() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
}

void turnMotorsOff() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
}
void getData() {
  // put the entire data signal into the data array
  for (int i = 0; i < 16; i = i + 1) {
    data[i] = digitalRead(2);
    
    delay(delayTime);
    Serial.println(data[i]);
  }
  Serial.println("data got");
  dataGot = 1;
}

void formatData() {
  // formats the code to make sure the activation code is at the front
  int yes = 1; 
  while (yes == 1) {
    // checks to see if the first 9 digits are the activation code
    Serial.println("loop time");
    int temp = data[0];
    for (int i = 1; i < 16; i = i + 1) {
      data[i - 1] = data[i];
    }
    data[15] = temp;
    Serial.println("looped");
    
    Serial.println("test");
    if (data[0]==activationCode[0]) {
      Serial.println("check 0");
      if (data[1]==activationCode[1]) {
        Serial.println("check 1");
        if (data[2]==activationCode[2]) {
          Serial.println("check 2");
          if (data[3]==activationCode[3]) {
            Serial.println("check 3");
            if (data[4]==activationCode[4]) {
              Serial.println("check 4");
              if (data[5]==activationCode[5]) {
                Serial.println("check 5");
                if (data[6]==activationCode[6]) {
                  Serial.println("check 6");
                  if (data[7]==activationCode[7]) {
                    Serial.println("check 7");
                    if (data[8]==activationCode[8]) {
                      Serial.println("check 8");
                      yes = 0;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    delay(1000);
  }
  
  for (int i = 9; i < 15; i = i + 1) {
    // puts the digits for the letter into its own seperate array
    letterCode[i - 9] = data[i];
  }
  Serial.println("data formatted");
}

void getNumber() {
  for (int i = 0; i < 7; i = i + 1) {
    letterCode[i] = data[i + 9];
  }
  Serial.println("letter separated");
  for (int i = 0; i < 7; i = i + 1) {
    Serial.print(letterCode[i]);
  }
  Serial.println("");
}


void convertNumber() {
  int exponent = 6;
  for (int i = 0; i < 7; i = i + 1) {
    if (letterCode[i] == 1) {
      theNumber = theNumber + pow(2, exponent);
    }
    exponent = exponent - 1;
  }
  theNumber = theNumber + 1;
  Serial.println(theNumber);
}

double sensorReading1 = 0; //sensor reading at theta 0
double sensorReading2 = 0; //sensor reading at theta 3.14/4
boolean runForward10 = true;
void loop() {
  // put your main code here, to run repeatedly:

  int sensorReadingStop = 0;

  double PAYLOAD_X = Enes100.destination.x;
  double PAYLOAD_Y = Enes100.destination.y;

  double arduinoAngle = atan((Enes100.destination.y - Enes100.location.y) / (Enes100.destination.x - Enes100.location.x)); //This is how the OSV will find the payload once it clears the obsticales
  int zeroChecker = 0; //Checks if the OSV had rotated over 0 for the second time
  int checkerStoper = 0; //Stops the OSV from rotating and grabbing sensor readings
  int firstRotationCheck = 0; //allows the OSV to travel back towards theta 0
  int beginSensorChecking = 0;


  Enes100.updateLocation();
  boolean straight = false;

  while (straight == false) {
    Enes100.updateLocation();
    if (Enes100.location.theta >= -.1 && Enes100.location.theta <= .1) {
      //      digitalWrite(left1, LOW);
      //      digitalWrite(left2, LOW);
      //      digitalWrite(right1, LOW);
      //      digitalWrite(right2, LOW);
      straight = true;
      beginSensorChecking = 1;
    } else if (Enes100.location.theta < -.1) {
      digitalWrite(left1, LOW);
      digitalWrite(left2, HIGH);
      analogWrite(leftPWM, 255);
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
      analogWrite(rightPWM, 255);
    } else if (Enes100.location.theta > .1) {
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
      analogWrite(leftPWM, 255);
      digitalWrite(right1, LOW);
      digitalWrite(right2, HIGH);
      analogWrite(rightPWM, 255);
    }
    Enes100.updateLocation();
  }

  //  if(straight == true && runForward10 == true){
  //  analogWrite(leftPWM, 255);
  //  analogWrite(rightPWM, 255);
  //  digitalWrite(left1, HIGH);
  //  digitalWrite(left2, LOW);
  //  digitalWrite(right1, HIGH);
  //  digitalWrite(right2, LOW);
  //  runForward10 = false;
  //  delay(8000);
  //  }
  //  if(straight == true && runForward10 == false){
  double goalX = 1000;
  goalX = Enes100.location.x + .2;
  while (Enes100.location.x < goalX) {
    Enes100.updateLocation();

    analogWrite(leftPWM, 255);
    analogWrite(rightPWM, 255);
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    sensorReading1 = readDistanceSensor1();
    sensorReading2 = readDistanceSensor2();

    if (sensorReading1 < 10 || sensorReading2 < 10) {
      break;
    }
    Enes100.updateLocation();
  }
  //  }




  //  while (checkerStoper == 0 && beginSensorChecking == 1) {
  //    Enes100.updateLocation();
  //    if (firstRotationCheck == 0 && Enes100.location.theta >= -.2 && Enes100.location.theta < (3.14 / 8)) {
  //      Enes100.println(abs(PAYLOAD_X - Enes100.location.x));
  //      digitalWrite(left1, LOW);
  //      digitalWrite(left2, HIGH);
  //      digitalWrite(right1, HIGH);
  //      digitalWrite(right2, LOW);
  //      Enes100.updateLocation();
  //      sensorReading1 = readDistanceSensor1();
  //      Enes100.print("The 1st sensor reading: ");
  //      Enes100.println(sensorReading1);
  //
  //    } else if (Enes100.location.theta >= (3.14 / 8) ) {
  //      digitalWrite(left1, HIGH);
  //      digitalWrite(left2, LOW);
  //      digitalWrite(right1, LOW);
  //      digitalWrite(right2, HIGH);
  //      Enes100.updateLocation();
  //      firstRotationCheck = 1;
  //      sensorReading2 = readDistanceSensor1();
  //      Enes100.print("The 2nd sensor reading: ");
  //      Enes100.println(sensorReading2);
  //    } else if (firstRotationCheck != 0 && Enes100.location.theta < -(3.14 / 8)) {
  //      digitalWrite(left1, LOW);
  //      digitalWrite(left2, HIGH);
  //      digitalWrite(right1, HIGH);
  //      digitalWrite(right2, LOW);
  //      Enes100.updateLocation();
  //      zeroChecker = 1;
  //      sensorReading3 = readDistanceSensor1();
  //      Enes100.print("The 3rd sensor reading: ");
  //      Enes100.println(sensorReading3);
  //    } else if (zeroChecker == 1 && Enes100.location.theta < .2 && Enes100.location.theta > -.2) {
  //      digitalWrite(left1, LOW);
  //      digitalWrite(left2, LOW);
  //      digitalWrite(right1, LOW);
  //      digitalWrite(right2, LOW);
  //      Enes100.updateLocation();
  //      Enes100.println("SHOULD END HERE!");
  //      checkerStoper = 1;
  //      break;
  //    } else
  //      Enes100.updateLocation();
  //
  //  }

  int checker = 0; //stops the turn loop
  int turnChecker = 0; //stops the first turn to go around the obstacle
  int turnOnce = 0; //stops the

  if (sensorReading1 <= 10 || sensorReading2 <= 10) {
    Enes100.println("UNDER 10 REACHED!");
    while (checker == 0) {
      boolean stopper = false;
      Enes100.updateLocation();
      if (Enes100.location.y < 1 && turnOnce == 0) {
        digitalWrite(left1, LOW);
        digitalWrite(left2, HIGH);
        digitalWrite(right1, HIGH);
        digitalWrite(right2, LOW);
        Enes100.updateLocation();
        turnChecker = 1;
        turnOnce = 1;
      }
      Enes100.updateLocation();
      if (Enes100.location.theta > ((3.14 / 2) - .2) && turnChecker == 1) {
        int sensorScannerLoop = 0;
        double sensorObstical = 0;
        double goalY = 1000;
        double goalX = 1000;

        goalY = Enes100.location.y + .55;
        while (Enes100.location.y < goalY) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        while (Enes100.location.theta > .2) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, LOW);
          digitalWrite(right2, HIGH);
          Enes100.updateLocation();
        }
        sensorReading1 = 50;
        sensorReading2 = 50;
        while (sensorReading1 > 7  || sensorReading2 > 7) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          sensorReading1 = readDistanceSensor1();
          sensorReading2 = readDistanceSensor2();
          Enes100.updateLocation();
          if (Enes100.location.x >= 2.5) {
            stopper = true;
            break;
          }
        }
        if (stopper) {
          break;
        }
        while (Enes100.location.theta > (-3.14 / 2) + .2) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, LOW);
          digitalWrite(right2, HIGH);
          Enes100.updateLocation();
        }
        goalY = Enes100.location.y - .55;
        while (Enes100.location.y > goalY) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        while ( Enes100.location.theta < -.2) {
          digitalWrite(left1, LOW);
          digitalWrite(left2, HIGH);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        sensorReading1 = readDistanceSensor1();
        sensorReading2 = readDistanceSensor2();
        break;
      }
      Enes100.updateLocation();
      if (Enes100.location.y > 1 && turnOnce == 0) {
        digitalWrite(left1, HIGH);
        digitalWrite(left2, LOW);
        digitalWrite(right1, LOW);
        digitalWrite(right2, HIGH);
        Enes100.updateLocation();
        turnChecker = 1;
        turnOnce = 1;
      }
      Enes100.updateLocation();
      if (Enes100.location.theta < (-(3.14 / 2) + .1) && turnChecker == 1) {
        int sensorScannerLoop = 0;
        double sensorObstical = 0;
        double goalY = 1000;
        double goalX = 1000;

        goalY = Enes100.location.y - .55;
        while (Enes100.location.y > goalY) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        while (Enes100.location.theta < -.2) {
          digitalWrite(left1, LOW);
          digitalWrite(left2, HIGH);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        sensorReading1 = 50;
        sensorReading2 = 50;
        while (sensorReading1 > 7 || sensorReading2 > 7) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          sensorReading1 = readDistanceSensor1();
          sensorReading2 = readDistanceSensor2();
          Enes100.updateLocation();
          if (Enes100.location.x >= 2.5) {
            stopper = true;
            break;
          }
        }
        if (stopper) {
          break;
        }
        while (Enes100.location.theta < 3.14 / 2 - .2) {
          digitalWrite(left1, LOW);
          digitalWrite(left2, HIGH);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        goalY = Enes100.location.y + .55;
        while (Enes100.location.y < goalY) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, HIGH);
          digitalWrite(right2, LOW);
          Enes100.updateLocation();
        }
        while (Enes100.location.theta > .2) {
          digitalWrite(left1, HIGH);
          digitalWrite(left2, LOW);
          digitalWrite(right1, LOW);
          digitalWrite(right2, HIGH);
          Enes100.updateLocation();
        }
        sensorReading1 = readDistanceSensor1();
        sensorReading2 = readDistanceSensor2();
        break;
      }
    }
    Enes100.updateLocation();

  }

  Enes100.updateLocation();
  if (abs(Enes100.destination.x - Enes100.location.x) <= .35) {
    Enes100.println("Messed Up Dawg");
    int slowDown = 0;
    int payLoadChecker = 0;
    analogWrite(leftPWM, 240);
    analogWrite(rightPWM, 240);
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    while (payLoadChecker == 0) {
      Enes100.updateLocation();
      Enes100.println(Enes100.location.theta);
      if (Enes100.location.theta < (atan((Enes100.destination.y - Enes100.location.y) / (Enes100.destination.x - Enes100.location.x))) + .1 && Enes100.location.theta > (atan((Enes100.destination.y - Enes100.location.y) / (Enes100.destination.x - Enes100.location.x))) - .1 && slowDown == 0) {
        digitalWrite(left1, HIGH);
        digitalWrite(left2, LOW);
        digitalWrite(right1, LOW);
        digitalWrite(right2, HIGH);
        slowDown = 1;
        Enes100.updateLocation();
      }
      Enes100.updateLocation();
      if (Enes100.location.theta < (atan((Enes100.destination.y - Enes100.location.y) / (Enes100.destination.x - Enes100.location.x))) + .1 && Enes100.location.theta > (atan((Enes100.destination.y - Enes100.location.y) / (Enes100.destination.x - Enes100.location.x))) - .1) {
        digitalWrite(left1, HIGH);
        digitalWrite(left2, LOW);
        digitalWrite(right1, HIGH);
        digitalWrite(right2, LOW);
      }
      Enes100.updateLocation();
      if ((Enes100.destination.x - Enes100.location.x) < .1 && abs(Enes100.destination.y - Enes100.location.y) < .1) {
        digitalWrite(left1, LOW);
        digitalWrite(left2, LOW);
        digitalWrite(right1, LOW);
        digitalWrite(right2, LOW);
        digitalWrite(talkToSlave, HIGH);

        delay(30000);
        //Do the arm thing
        digitalWrite(left1, HIGH);
        digitalWrite(left2, LOW);
        digitalWrite(right1, HIGH);
        digitalWrite(right2, LOW);
        delay(300);
        digitalWrite(left1, LOW);
        digitalWrite(left2, LOW);
        digitalWrite(right1, LOW);
        digitalWrite(right2, LOW);
        digitalWrite(talkToSlave, LOW);
//        while (digitalRead(dataIn) != 1) {
//          delay(100);
//        }
//        
//        theNumber = 0;
//        dataGot = 0;
//        delay(2000);
//        for (int i = 0; i < 9; i = i + 1) {
//          Serial.println(activationCode[i]);
//        }
//        delay(1000);
//        Serial.println("delayed");
//        while (dataGot == 0) {
//          getData();
//        }
//
//        formatData();
//
//        for (int i = 0; i < 16; i = i + 1) {
//          Serial.print(data[i]);
//        }
//        Serial.print("");
//
//        getNumber();
//
//        convertNumber();
//
//        char letter = theNumber;
//        Enes100.mission(letter);
        
        

        
        //do the extract thing
        digitalWrite(left1, LOW);
        digitalWrite(left2, HIGH);
        digitalWrite(right1, LOW);
        digitalWrite(right2, HIGH);
        delay(2000);
        digitalWrite(left1, LOW);
        digitalWrite(left2, LOW);
        digitalWrite(right1, LOW);
        digitalWrite(right2, LOW);

        //put the arm down
        digitalWrite(left1, LOW);
        digitalWrite(left2, HIGH);
        digitalWrite(right1, LOW);
        digitalWrite(right2, HIGH);
        delay(2000);
        //draw dick in sand
      }

    }
  }


}
