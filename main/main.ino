#include "buffer.hpp"
#include "20_110filter.h"     //filter coeficcients

//============
// Definitions
//============

volatile unsigned long currentRead;
volatile unsigned long computeFlag;
unsigned long currentReadCopy;
unsigned long computeFlagCopy;
IntervalTimer interruptTimer;


int phaseDetect[4] = {0, 0, 0, 0};            //keeps track of what phase each filter is at
double amplitudes[4] = {0.0, 0.0, 0.0, 0.0};  //keeps track of what amplitude each filter is at

#define ADC_PIN A7
#define SAMPLING_RATE 500   //for 2kHz or 0.0005 s
#define default_thresh 100 //Remember, the max is 514 here!

String entry;
int triggering_faze = 0;
int global_thresh = 100;
int currentbest = 0;
bool UI_done_flag = false;

unsigned current_bank = 0;
unsigned alt_bank_counter = 0;
#define alt_bank_thresh 50        //how many samples the other must be bigger for

//===========
// Prototypes
//===========

void readADC(void);
int getMaxAmplitude(double* amplitudes, unsigned* current, unsigned* counter);
void welcome();
void user_interface();

//===========
// Buffers
//===========

//initializes filters
jd::buffer band_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, default_thresh, &phaseDetect[0]);
jd::buffer band_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, default_thresh, &phaseDetect[1]);
jd::buffer band_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, default_thresh, &phaseDetect[2]);
jd::buffer band_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, default_thresh, &phaseDetect[3]);

//===========
// Setup
//===========

void setup() {
  /*Serial.begin(9600);
    delay(5000);
    while (!Serial);*/
  pinMode(ADC_PIN, INPUT);
  interruptTimer.begin(readADC, SAMPLING_RATE);
}

//==========
// Main Loop
//==========

void loop() {

  //===============
  // User Interface
  //===============
  if (Serial.available() && UI_done_flag == false) {
    user_interface();
  } else {
   
    //====================================================
    // Signal processing. Occurs when no serial plugged in
    //====================================================

    //gets ADC (or read from vector at 2 kHz
    noInterrupts();
    computeFlagCopy = computeFlag;
    currentReadCopy = currentRead;
    interrupts();

    //if we read a new value: do stuff. else: dont do stuff
    if (computeFlagCopy == 1) {

      //adds new read to each band and updates the amplitude readings
      amplitudes[0] = band_20_50.insert((double)currentReadCopy);
      amplitudes[1] = band_40_70.insert((double)currentReadCopy);
      amplitudes[2] = band_60_90.insert((double)currentReadCopy);
      amplitudes[3] = band_80_110.insert((double)currentReadCopy);

      //NOT AGAIN!!
      noInterrupts();
      computeFlag = 0;
      interrupts();

      //ADD TRIGGERING
      currentbest = getMaxAmplitude(amplitudes, &current_bank, &alt_bank_counter);
    }
  }
}

//=========
//Functions
//=========

//decides which bank has the highest amplitude
int getMaxAmplitude(double* amplitudes, unsigned* current, unsigned* counter) {
  double maxx = 0.0;
  int maxind = 0;
  for (int i = 0; i < 4; i++) {
    if (amplitudes[i] > maxx) {
      maxx = amplitudes[i];
      maxind = i;
    }
  }

  //back switching things
  if (maxind == *current) {
    *counter = 0;
  } else {
    if (*counter < alt_bank_thresh) {
      (*counter)++;
      maxind = *current;
    } else {
      counter = 0;
      *current = maxind;
    }
  }

  return maxind;
}

void readADC(void) {
  currentRead = analogRead(ADC_PIN);
  computeFlag = 1;
}

void welcome() {
  Serial.println("===============");
  Serial.println("Welcome to teensy interface for closed loop gamma modulation");
  Serial.println();
  Serial.println("Possible options:");
  Serial.println("\tphase: adjust which phase you desire for triggering");
  Serial.println("\t\t0 -> -/+ 0 cross");
  Serial.println("\t\t1 -> peak,");
  Serial.println("\t\t2 -> +/- 0 cross");
  Serial.println("\t\t3 -> trough");
  Serial.println("\tthreshold: determines the amplitude threshold required for triggering");
  Serial.println("\t\t0-514 with 514 being +/- 3.3 V");
  Serial.println("\tquit: exit this menu");
  Serial.println("===============");
}

void user_interface() {

  //uses global variables so could be a bit sketchy
  entry = Serial.readStringUntil('\n');
  welcome();
  while (!entry.equals("quit") ) {
    if (entry.equals("phase")) {
      Serial.print("Phase is currently set at ");
      Serial.println(triggering_faze);
      Serial.print("Enter a new value: ");
      while (1) {
        if (Serial.available()) {
          String new_val = Serial.readStringUntil('\n');
          triggering_faze = new_val.toInt();
          break;
        }
      }
      Serial.println();
      Serial.print("Phase set to ");
      Serial.println(triggering_faze);
      entry = String("job_complete");
    } else if (entry.equals("threshold")) {
      Serial.print("Threshold is currently set at ");
      Serial.println(global_thresh);
      Serial.print("Enter a new value: ");
      while (1) {
        if (Serial.available()) {
          String new_val = Serial.readStringUntil('\n');
          global_thresh = new_val.toInt();
          break;

          //NOTE: NEED TO ACTUALLY CHANGE THE THRESHOLD HERE
        }
      }
      Serial.println();
      Serial.print("Threshold set to ");
      Serial.println(global_thresh);
      entry = String("job_complete");
    } else if (entry.equals("job_complete")) {
      Serial.print("Enter a new option: ");
      while (1) {
        if (Serial.available()) {
          entry = Serial.readStringUntil('\n');
          break;
        }
      }
    } else {
      Serial.println("Incorrect entry. Try again");
      Serial.println();
      //while (!entry.equals("quit") || !entry.equals("phase") || entry.equals("threshold")) {
      while (1) {
        if (Serial.available()) {
          entry = Serial.readStringUntil('\n');
          break;
        }
      }
    }
  }
  UI_done_flag = true;
  band_20_50.restart((double)global_thresh);
  band_40_70.restart((double)global_thresh);
  band_60_90.restart((double)global_thresh);
  band_80_110.restart((double)global_thresh);

  Serial.println("Entering main code. Goodbye");
}
