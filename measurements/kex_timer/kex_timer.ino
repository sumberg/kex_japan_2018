#define INSTRS 5000
#define INSTR_INCR 10
#define DEF_INSTRS 1000
#define TIMER_OVERFLOW_MAX 140 // (180 - 40)

/* Used pins */
int stopPin, startPin, waitPin, btnPin;
/* Test modes */
enum MODE {DEF, INCR_LENGTH, INCR_RESET, NO_EMU_ROM_FUNC, MODE_DONE} mode;
/* Test category (IMM, ZP, ABS, MIX) */
int category;

void waitForSlaveSetup(void);
void setupPins(void);
void printCategory(void);
double measureTime(void);

void setup() {
  setupPins();
  mode = DEF;
  category = 0;

  /* Start serial monitor at baud rate 9600 */
  Serial.begin(9600);
  
  /* Wait for ATmega328P to signal for ready */
  waitForSlaveSetup();  
}

void loop() {
  boolean done;
  /* Wait for button press to start tests */
  Serial.println("Press button to start . . .\n");
  while (digitalRead(btnPin) != HIGH);

  if (category == 4) {
    /* If all categories are done, do not enter test loop */
    Serial.println("All tests done!");
    done = true;
  } else {
    /* Otherwise, set relevant variabels to their default values */
    done = false;
    mode = DEF;
  }

  printCategory();

  /* Main test loop */
  while(!done) {
      int count;
      double tRes;
      switch(mode) {
          case DEF :
              Serial.println("Test case 1: All values default.");
              tRes = measureTime();
              /* Print to serial monitor */
              Serial.print(DEF_INSTRS);
              Serial.print(" instr: ");
              Serial.println(tRes);
              mode = INCR_LENGTH;
              break;
      
          case INCR_LENGTH :
              count = 0;
              Serial.println("Test case 2: Increase # of instructions from 1 to 5000 in increments of 10.");
              while(count < INSTRS) {
                  tRes = measureTime();
                  /* Print to serial monitor */
                  Serial.print(count += INSTR_INCR);
                  Serial.print(" ");
                  Serial.println(tRes);
              }
              mode = INCR_RESET;
              break;
                          
          case INCR_RESET :
              count = 0;
              Serial.println("Test case 3: Increase # of timer overflows from 40 to 180 in increments of 1.");
              Serial.println("Reset PC function call frequency = (System clock / Prescaler / Timer resolution / # of timer overflows) =");
              Serial.println("20 MHz / 8 / 256 / # of timer overflows");
              while (count < TIMER_OVERFLOW_MAX) {
                  tRes = measureTime();
                  /* Print to serial monitor */
                  Serial.print(count + 140);
                  Serial.print(" ");
                  Serial.println(tRes);
                  count++;
              }
              mode = NO_EMU_ROM_FUNC;
              break;
      
           case NO_EMU_ROM_FUNC :
              /* TODO */
              mode = MODE_DONE;
              break;
           case MODE_DONE :
              done = true;
              category++;
        }
    }
}

/* Wait for 2A03 to signal for ready */
void waitForSlaveSetup() {
  Serial.println("Waiting for ATmega328P setup . . .");
  while(digitalRead(waitPin) != HIGH);
  Serial.println("Going!\n");
  delay(1000);
}

/* Setup used Arduino M0 Pro pins */
void setupPins() {
  waitPin = 5, startPin = 6, stopPin = 7, btnPin = 12;
  pinMode(startPin, OUTPUT);
  pinMode(stopPin, INPUT);
  pinMode(btnPin, INPUT);
  pinMode(waitPin, INPUT);
}

/* Print current testing category */
void printCategory() {
  Serial.print("Category: ");
  switch(category) {
    case 0 :  Serial.println("Immediate instructions");
              break;
    case 1 :  Serial.println("Zero page instructions");
              break;
    case 2 :  Serial.println("Absolute instructions");
              break;
    case 3 :  Serial.println("Mixed instructions");
              break;
  }
}

/* Measure time it takes for 2A03 to complete its set task (based on mode and category) */
double measureTime() {
  double tStart, tEnd;
  /* Start ATmega328P execution */
  digitalWrite(startPin, HIGH);
  /* Start timer */
  tStart = micros();
  /* Wait for Atmega328p to finish execution */
  while(digitalRead(stopPin) != HIGH);
  /* Stop timer */
  tEnd = micros();
  /* End ATMega328P execution */
  digitalWrite(startPin, LOW);
  /* Calculate and return elapsed time (ms) */
  return (tEnd - tStart) / 1000;
}
