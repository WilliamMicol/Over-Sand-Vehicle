int trigPin1 = 2;
int echoPin1 = 4;
int trigPin2 = 10;
int echoPin2 = 9;
int distance1;
int distance2;
double duration1;
double duration2;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Serial.begin(9600);
}

int readDistanceSensor1(){
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
//      Serial.print("Distance1: ");
//      Serial.println(distance1);
  return distance1;
}

int readDistanceSensor2(){
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
//      Serial.print("Distance2: ");
//      Serial.println(distance2);
  return distance2;
}

void loop() {

  Serial.print("Distance1: ");
  Serial.println(readDistanceSensor1());
  delay(1000);
  Serial.print("Distance2: ");
  Serial.println(readDistanceSensor2());
  delay(1000);


}
