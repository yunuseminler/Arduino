int ses_sensoru = 2;
int role = 5;
 
void setup() {
  pinMode(ses_sensoru, INPUT);
  pinMode(role, OUTPUT);
}
 
void loop() {
 
  int sensor_durumu = digitalRead(ses_sensoru);
 
  if (sensor_durumu == 1)
  {
    digitalWrite(role,HIGH);
    
  }
 
  else if (sensor_durumu == 0)
  {
    digitalWrite(role,HIGH);
  }
}
