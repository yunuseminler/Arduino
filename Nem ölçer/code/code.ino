int sensorPin = 9;                  
int buzzerPin = 8;                  
int veri;                          

void setup() {
  pinMode(sensorPin, INPUT);       
  pinMode(buzzerPin, OUTPUT);      
}
void loop() {
  veri = digitalRead(sensorPin);    
  if(veri == true){           .
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  else{                            
    digitalWrite(buzzerPin, LOW);
  }
}
