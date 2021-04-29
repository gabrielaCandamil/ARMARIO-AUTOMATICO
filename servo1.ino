#include <Pulses.h>

#define PIN_STEPPER_PUL 3
#define PIN_STEPPER_DIR 2
#define PIN_STEPPER_ENA 50
#define STEPPER_PULSES_PER_REV 1600UL
#define FREQ (300UL) // Hz
#define NUM_REVOLUTIONS 4
#define STEPPER_STEPS ((NUM_REVOLUTIONS) * (STEPPER_PULSES_PER_REV))
const int pinStop = 21;
const int pinRight = 4;
const int pinLeft = 5;

int btnLeft, btnRight, cntL, cntR = 0;
int cntRight, cntLeft = 0;
volatile uint32_t stepsCnt = 0UL;

ISR(TIMER3_COMPA_vect) {
  if (stepsCnt) {
    --stepsCnt;
    if (stepsCnt == 0UL) {
      stopPulses(PIN_STEPPER_PUL);
    }
  }
}
void interruptStop(){
    stopPulses(PIN_STEPPER_PUL);
    cntRight = 0;
    cntLeft = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(PIN_STEPPER_DIR, OUTPUT);
  pinMode(PIN_STEPPER_PUL, OUTPUT);
  pinMode(PIN_STEPPER_ENA, OUTPUT);
  pinMode(pinStop, INPUT);
  pinMode(pinLeft, INPUT);
  pinMode(pinRight, INPUT);
  pinMode(7, OUTPUT);
  
  //digitalWrite(PIN_STEPPER_ENA, HIGH);
  attachInterrupt(digitalPinToInterrupt(pinStop), interruptStop, RISING);
  // It depends on the PIN_STEPPER_PUL
  TIMSK3 |= 1 << OCIE3A;
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  btnRight = digitalRead(pinRight);
  btnLeft = digitalRead(pinLeft);
  
  if (btnRight == HIGH && btnLeft == LOW && cntRight == 0 && cntLeft == 0 && cntR == 0) {
    if (stepsCnt == 0UL){
      stepsCnt = STEPPER_STEPS;
    }
    digitalWrite(PIN_STEPPER_DIR, HIGH);
    startPulses(PIN_STEPPER_PUL, FREQ, 3);
    cntRight = 1;
  }
  if (stepsCnt == 0UL && cntRight == 1){
    cntRight = 0;
    digitalWrite(PIN_STEPPER_DIR, LOW);
    cntR = 1;
    cntL = 0;
  }
  
  if (btnLeft == HIGH && btnRight == LOW && cntLeft == 0 && cntRight == 0 && cntL == 0) {
    if (stepsCnt == 0UL){
      stepsCnt = STEPPER_STEPS;
    }
    digitalWrite(PIN_STEPPER_DIR, LOW);
    startPulses(PIN_STEPPER_PUL, FREQ, 3);
    cntLeft = 1;
  } 
  if(stepsCnt == 0UL && cntLeft == 1){
    cntLeft = 0;
    digitalWrite(PIN_STEPPER_DIR, HIGH);
    cntL = 1;
    cntR = 0;
  }
}
