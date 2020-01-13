#include "Enes100.h"
void setup() {
  // put your setup code here, to run once:
  Enes100.begin("No Cap", DATA, 13, 12, 11);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Enes100.ping() == true){
    Serial.println("works");
  } else{
    Serial.println("Fuck");
  }

}
