#define INSTRS 5000

double tStart, tEnd, tRes;
int count;
int endPin, startPin, ledPin, waitPin;

void setup() {
  tStart = 0, tEnd = 0, tRes = 0;
  waitPin = 5, startPin = 6, endPin = 7, ledPin = 12;
  count = 0;
  
  pinMode(startPin, OUTPUT);
  pinMode(endPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(waitPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  /* Wait for Atmega328p to signal for ready */
  Serial.println("Waiting . . .");
  wait();
  Serial.println("Going!");
  
  delay(1000);
  count = 0;
  
  while(count < INSTRS) {
    /* Start Atmega328p execution */
    digitalWrite(startPin, HIGH);
    tStart = micros();
    
    /* Wait for Atmega328p to finish execution */
    while(digitalRead(endPin) != HIGH);
    tEnd = micros();
    digitalWrite(startPin, LOW);
    
    /* Calculate elapsed time (ms) */
    tRes = (tEnd - tStart) / 1000;
    
    /* Print to serial monitor */
    Serial.print(count += 10);
    Serial.print(" ");
    Serial.println(tRes);
  }
  
  /* Idle indefinitely */
  Serial.println("Done!");
  while(1);
}

void wait() {
  toggleLED();
  while(digitalRead(waitPin) != HIGH);
  toggleLED();
}

void toggleLED() {
  digitalWrite(ledPin, !digitalRead(ledPin));
}

