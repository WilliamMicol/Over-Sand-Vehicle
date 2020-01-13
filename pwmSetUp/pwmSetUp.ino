int rearRight1 = 0;
int rearRight2 = 1;
int rearRightpwm = 3;
int frontRight1 = 2;
int frontRight2 = 4;
int frontRightpwm = 5;
int frontLeft1 = 7;
int frontLeft2 = 8;
int frontLeftpwm = 6;
int rearLeft1 = 9;
int rearLeft2 = 10;
int rearLeftpwm = 11;

void setup() {
  pinMode(rearRight1, OUTPUT);
  pinMode(rearRight2, OUTPUT);
  pinMode(rearRightpwm, OUTPUT);
  pinMode(frontRight1, OUTPUT);
  pinMode(frontRight2, OUTPUT);
  pinMode(frontRightpwm, OUTPUT);
  pinMode(frontLeft1, OUTPUT);
  pinMode(frontLeft2, OUTPUT);
  pinMode(frontLeftpwm, OUTPUT);
  pinMode(rearRigth1, OUTPUT);
  pinMode(rearRigth2, OUTPUT);
  pinMode(rearRightpwm, OUTPUT);
  
}

void loop() {
  //go forward for 2 seconds
  digitalWrite(rearRight1, HIGH);
  digitalWrite(rearRight2, LOW);
  analogWrite(rearRightpwm, 150);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  analogWrite(frontRightpwm, 150);
  digitalWrite(rearLeft1, HIGH);
  digitalWrite(rearLeft2, LOW);
  analogWrite(rearLefttpwm, 150);
  digitalWrite(frontLeft1, HIGH);
  digitalWrite(frontLeft2, LOW);
  analogWrite(frontLeftpwm, 150);
  delay(2000);
  //go backwards for 2 seconds
  digitalWrite(rearRight1, LOW);
  digitalWrite(rearRight2, HIGH);
  digitalWrite(frontRight1, LOW);
  digitalWrite(frontRight2, HIGH);
  digitalWrite(rearLeft1, LOW);
  digitalWrite(rearLeft2, HIGH);
  digitalWrite(frontLeft1, LOW);
  digitalWrite(frontLeft2, HIGH);
  delay(2000);
  //spin left for 2 seconds
  digitalWrite(rearRight1, HIGH);
  digitalWrite(rearRight2, LOW);
  digitalWrite(frontRight1, HIGH);
  digitalWrite(frontRight2, LOW);
  delay(2000);


}
