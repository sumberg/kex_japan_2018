#define INSTRS 100

double tStart, tEnd, tRes;
int count;
int endPin, startPin, waitPin, btnPin;

void setup() {
  tStart = 0, tEnd = 0, tRes = 0;
  waitPin = 5, startPin = 6, endPin = 7, btnPin = 12;
  count = 0;
  
  pinMode(startPin, OUTPUT);
  pinMode(endPin, INPUT);
  pinMode(btnPin, INPUT);
  pinMode(waitPin, INPUT);
  
  Serial.begin(9600);
  
  /* Wait for Atmega328p to signal for ready */
  Serial.println("Waiting for ATmega328P setup . . .");
  wait();
  Serial.println("Going!\n");
  
  delay(1000);
}

void loop() {
  count = 0;
  Serial.println("Press button to start . . .");
  Serial.println();
  while (digitalRead(btnPin) != HIGH);

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
}

void wait() {
  while(digitalRead(waitPin) != HIGH);
}
