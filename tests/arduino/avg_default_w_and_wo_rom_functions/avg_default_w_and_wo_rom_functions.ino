#define INSTRS_MAX 5000
#define INSTR_INCR 10
#define INSTRS_DEFAULT 1000
#define LAPS 50

/* Pins */
int slaveSetupWaitPin, startSlaveExecPin, stopSlaveExecPin, btnPin, slaveWaiting;
double times[LAPS];

void waitForSlaveSetup(void);
void setupPins(void);
void printCategory(void);
void waitForBtnPress();
double measureTime(void);

void setup() {
  /* Setup pin modes */
  setupPins();
  /* Start serial monitor at baud rate 9600 */
  Serial.begin(9600);
  /* Wait for ATmega328P to signal for ready */
  waitForSlaveSetup();
}

void loop() {
  /* Wait for button press to start tests */
  waitForBtnPress();
  
  /* Main test loop */
  Serial.println("Running. . .");
  for (int i = 0; i < LAPS; i++) {
    times[i] = measureTime();
  }

  /* Calculate averages and print */
  printAvg(calculateAverages());
 
  Serial.println("Test program completed!");
  /* Suspend execution */
  while(1);
}

/* Calculate averages */
double calculateAverages() {
  double avg = 0;
  for (int i = 0; i < LAPS; i++) {
    avg += times[i];
  }
  return (double) avg / LAPS;
}

/* Print average */
void printAvg(double avg) {
    Serial.print("The average execution time for ");
    Serial.print(INSTRS_DEFAULT);
    Serial.print(" instructions is ");
    Serial.println(avg);
    Serial.println();
}

/* Wait for 2A03 to signal for ready */
void waitForSlaveSetup() {
  Serial.println("Waiting for ATmega328P setup . . .");
  while(digitalRead(slaveSetupWaitPin) != HIGH);
  Serial.println("Slave ready, starting program!\n");
  delay(1000);
}

/* Setup used Arduino M0 Pro pins */
void setupPins() {
  slaveSetupWaitPin = 5, startSlaveExecPin = 6,
  stopSlaveExecPin = 7, btnPin = 12, slaveWaiting = 4;
  pinMode(slaveSetupWaitPin, INPUT);
  pinMode(startSlaveExecPin, OUTPUT);
  pinMode(stopSlaveExecPin, INPUT);
  pinMode(btnPin, INPUT);
  pinMode(slaveWaiting, INPUT);
}

void waitForBtnPress() {
  Serial.println("Press button to start . . .");
  while (digitalRead(btnPin) != HIGH);
  Serial.println("Button pressed!\n");
}

/* Measure time it takes for 2A03 to complete its set task (based on mode and category) */
double measureTime() {
  double tStart, tEnd;
  /* Wait for ATMega328P to be ready for start signal */
  while (digitalRead(slaveWaiting) != HIGH);
  /* Start ATmega328P execution */
  digitalWrite(startSlaveExecPin, HIGH);
  /* Start timer */
  tStart = micros();
  /* Wait for Atmega328p to finish execution */
  while(digitalRead(stopSlaveExecPin) != HIGH);
  /* End ATMega328P execution */
  digitalWrite(startSlaveExecPin, LOW);
  /* Stop timer */
  tEnd = micros();
  
  /* Calculate and return elapsed time (ms) */
  return (tEnd - tStart) / 1000.0;
}
