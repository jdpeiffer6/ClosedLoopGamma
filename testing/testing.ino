//Hello and welcome to the testing script! This is useful for visualizing the output from the dataset with a predefined input!
//Instead of reading from the ADC, this scrip "loads" a file with a very long vector of sinusoidal data.
//It then ouputs that data along with filter information to the serial terminal. This can be read and saved to a file for later analysis.
//If you are interested in making your own input data, simply make a vector in matlab and view the format for saving in "test.h"
//Updated 1/27/2021
//J.D. Peiffer

//============
// Definitions
//============

unsigned current_bank = 0;
unsigned alt_bank_counter = 0;
#define alt_bank_thresh 50        //how many samples the other must be bigger for

#include "buffer.hpp"
#include "20_110filter.h"     //filter coeficcients
#include "test_SNR4.h"             //test dataset. instead of reading from ADC, just read from this vector
#include "test_SNR8.h"
#include "test_SNR12.h"
#include "test_SNR16.h"

#define thresh 100 //TRIGGERING THRESHOLD! Range of 0-512

//Data locations that filters will continously update
int phaseDetect4[4] = {0, 0, 0, 0};            //keeps track of what phase each filter is at
double amplitudes4[4] = {0.0, 0.0, 0.0, 0.0};  //keeps track of what amplitude each filter is at

int phaseDetect8[4] = {0, 0, 0, 0};
double amplitudes8[4] = {0.0, 0.0, 0.0, 0.0};

int phaseDetect12[4] = {0, 0, 0, 0};
double amplitudes12[4] = {0.0, 0.0, 0.0, 0.0};

int phaseDetect16[4] = {0, 0, 0, 0};
double amplitudes16[4] = {0.0, 0.0, 0.0, 0.0};

//triggering
bool triggered = false;
unsigned trigger_counter = 0;
unsigned duration_limit = 10;
int triggering_faze = 2;
//===========
// Prototypes
//===========

int getMaxAmplitude(double* amplitudes, unsigned* current, unsigned* counter);

//===========
// Buffers
//===========

jd::buffer band4_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect4[0]);
jd::buffer band4_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect4[1]);
jd::buffer band4_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect4[2]);
jd::buffer band4_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect4[3]);

jd::buffer band8_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect8[0]);
jd::buffer band8_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect8[1]);
jd::buffer band8_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect8[2]);
jd::buffer band8_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect8[3]);

jd::buffer band12_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect12[0]);
jd::buffer band12_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect12[1]);
jd::buffer band12_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect12[2]);
jd::buffer band12_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect12[3]);

jd::buffer band16_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect16[0]);
jd::buffer band16_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect16[1]);
jd::buffer band16_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect16[2]);
jd::buffer band16_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect16[3]);

//===========
// Setup
//===========

void setup() {
  Serial.begin(9600);
  delay(5000);
  while (!Serial);

  //Each line of the serial output will read: [X (input vector), 35 Hz centered phase filter, 35 Hz centered amplitude filter, ...and so on for 55, 75, 95, Filter bank with strongest signal, Detected phase]
  Serial.println("X,35Phase,35Amplitude,55Phase,55Amplitude,75Phase,75Amplitude,95Phase,95Amplitude,CurrentBest,PhaseDetect");

}
//==========
// Main Loop
//==========
int* x = test_SNR4;
int currentbest = 0;
int SNR = 4;
void loop() {

  //=====
  //SNR 4
  //=====

  for (int jd2 = 0; jd2 < LEN; jd2++) {
    //adds new read to each band and updates the amplitude readings
    amplitudes4[0] = band4_20_50.insert((double)x[jd2]);
    amplitudes4[1] = band4_40_70.insert((double)x[jd2]);
    amplitudes4[2] = band4_60_90.insert((double)x[jd2]);
    amplitudes4[3] = band4_80_110.insert((double)x[jd2]);

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(x[jd2]);
    Serial.print(',');
    band4_20_50.plt(0);
    band4_40_70.plt(0);
    band4_60_90.plt(0);
    band4_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes4, &current_bank, &alt_bank_counter);
    if (triggered == true) {
      if (trigger_counter > duration_limit) {
        //digitalWrite(TRIGGERING_PIN, LOW);
        trigger_counter = 0;
        triggered = false;
      } else {
        trigger_counter++;
      }
    } else {
      if (phaseDetect4[currentbest] == triggering_faze) {
        //trigger
        //digitalWrite(TRIGGERING_PIN, HIGH);
        triggered = true;
      }
    }

    
    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect4[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes4[0]);
    Serial.print(',');
    Serial.print(amplitudes4[1]);
    Serial.print(',');
    Serial.print(amplitudes4[2]);
    Serial.print(',');
    Serial.print(amplitudes4[3]);
    Serial.print(',');
    Serial.print(SNR);
    Serial.print(',');
    Serial.print(jd2);
    Serial.print(',');
    if (triggered==true) {
      Serial.println(1);
    } else {
      Serial.println(0);
    }
    delay(1);
  }

  //=====
  //SNR 8
  //=====

  //added
  band4_20_50.restart(thresh);
  band4_40_70.restart(thresh);
  band4_60_90.restart(thresh);
  band4_80_110.restart(thresh);



  SNR = 8;
  x = test_SNR8;
  currentbest = 0;

  //added
  amplitudes4[0] = 0; amplitudes4[1] = 0; amplitudes4[2] = 0; amplitudes4[3] = 0;
  for (int jd2 = 0; jd2 < LEN; jd2++) {
    //adds new read to each band and updates the amplitude readings
    amplitudes4[0] = band4_20_50.insert((double)x[jd2]);
    amplitudes4[1] = band4_40_70.insert((double)x[jd2]);
    amplitudes4[2] = band4_60_90.insert((double)x[jd2]);
    amplitudes4[3] = band4_80_110.insert((double)x[jd2]);

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(x[jd2]);
    Serial.print(',');
    band4_20_50.plt(0);
    band4_40_70.plt(0);
    band4_60_90.plt(0);
    band4_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes4, &current_bank, &alt_bank_counter);

    //triggering
    if (triggered == true) {
      if (trigger_counter > duration_limit) {
        //digitalWrite(TRIGGERING_PIN, LOW);
        trigger_counter = 0;
        triggered = false;
      } else {
        trigger_counter++;
      }
    } else {
      if (phaseDetect4[currentbest] == triggering_faze) {
        //trigger
        //digitalWrite(TRIGGERING_PIN, HIGH);
        triggered = true;
      }
    }


    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect4[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes4[0]);
    Serial.print(',');
    Serial.print(amplitudes4[1]);
    Serial.print(',');
    Serial.print(amplitudes4[2]);
    Serial.print(',');
    Serial.print(amplitudes4[3]);
    Serial.print(',');
    Serial.print(SNR);
    Serial.print(',');
    Serial.print(jd2);
    Serial.print(',');
    if (triggered==true) {
      Serial.println(1);
    } else {
      Serial.println(0);
    }
    delay(1);
  }
  /*
    for (int jd2 = 0; jd2 < LEN; jd2++) {
    //adds new read to each band and updates the amplitude readings
    amplitudes8[0] = band8_20_50.insert((double)x[jd2]);
    amplitudes8[1] = band8_40_70.insert((double)x[jd2]);
    amplitudes8[2] = band8_60_90.insert((double)x[jd2]);
    amplitudes8[3] = band8_80_110.insert((double)x[jd2]);

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(x[jd2]);
    Serial.print(',');
    band8_20_50.plt(0);
    band8_40_70.plt(0);
    band8_60_90.plt(0);
    band8_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes8, &current_bank, &alt_bank_counter);
    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect8[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes8[0]);
    Serial.print(',');
    Serial.print(amplitudes8[1]);
    Serial.print(',');
    Serial.print(amplitudes8[2]);
    Serial.print(',');
    Serial.print(amplitudes8[3]);
    Serial.print(',');
    Serial.print(SNR);
    Serial.print(',');
    Serial.println(jd2);
    delay(1);
    }*/

  //======
  //SNR 12
  //======

  SNR = 12;
  x = test_SNR12;
  currentbest = 0;
  triggered=false;
  trigger_counter=0;
  
  for (int jd2 = 0; jd2 < LEN; jd2++) {
    //adds new read to each band and updates the amplitude readings
    amplitudes12[0] = band12_20_50.insert((double)x[jd2]);
    amplitudes12[1] = band12_40_70.insert((double)x[jd2]);
    amplitudes12[2] = band12_60_90.insert((double)x[jd2]);
    amplitudes12[3] = band12_80_110.insert((double)x[jd2]);

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(x[jd2]);
    Serial.print(',');
    band12_20_50.plt(0);
    band12_40_70.plt(0);
    band12_60_90.plt(0);
    band12_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes12, &current_bank, &alt_bank_counter);

    if (triggered == true) {
      if (trigger_counter > duration_limit) {
        //digitalWrite(TRIGGERING_PIN, LOW);
        trigger_counter = 0;
        triggered = false;
      } else {
        trigger_counter++;
      }
    } else {
      if (phaseDetect12[currentbest] == triggering_faze) {
        //trigger
        //digitalWrite(TRIGGERING_PIN, HIGH);
        triggered = true;
      }
    }
    
    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect12[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes12[0]);
    Serial.print(',');
    Serial.print(amplitudes12[1]);
    Serial.print(',');
    Serial.print(amplitudes12[2]);
    Serial.print(',');
    Serial.print(amplitudes12[3]);
    Serial.print(',');
    Serial.print(SNR);
    Serial.print(',');
    Serial.print(jd2);
    Serial.print(',');
    if (triggered==true) {
      Serial.println(1);
    } else {
      Serial.println(0);
    }
    delay(1);
  }

  //======
  //SNR 16
  //======

  SNR = 16;
  x = test_SNR16;
  currentbest = 0;
  triggered=false;
  trigger_counter=0;
  
  for (int jd2 = 0; jd2 < LEN; jd2++) {
    //adds new read to each band and updates the amplitude readings
    amplitudes16[0] = band16_20_50.insert((double)x[jd2]);
    amplitudes16[1] = band16_40_70.insert((double)x[jd2]);
    amplitudes16[2] = band16_60_90.insert((double)x[jd2]);
    amplitudes16[3] = band16_80_110.insert((double)x[jd2]);

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(x[jd2]);
    Serial.print(',');
    band16_20_50.plt(0);
    band16_40_70.plt(0);
    band16_60_90.plt(0);
    band16_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes16, &current_bank, &alt_bank_counter);

    if (triggered == true) {
      if (trigger_counter > duration_limit) {
        //digitalWrite(TRIGGERING_PIN, LOW);
        trigger_counter = 0;
        triggered = false;
      } else {
        trigger_counter++;
      }
    } else {
      if (phaseDetect16[currentbest] == triggering_faze) {
        //trigger
        //digitalWrite(TRIGGERING_PIN, HIGH);
        triggered = true;
      }
    }
    
    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect16[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes16[0]);
    Serial.print(',');
    Serial.print(amplitudes16[1]);
    Serial.print(',');
    Serial.print(amplitudes16[2]);
    Serial.print(',');
    Serial.print(amplitudes16[3]);
    Serial.print(',');
    Serial.print(SNR);
    Serial.print(',');
    Serial.print(jd2);
    Serial.print(',');
    if (triggered==true) {
      Serial.println(1);
    } else {
      Serial.println(0);
    }
    delay(1);
  }

  while (1) {
    delay(100);
  }
}


//=============
//End Main Loop
//=============


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
