//Hello and welcome to the testing script! This is useful for visualizing the output from the dataset with a predefined input!
//Instead of reading from the ADC, this scrip "loads" a file with a very long vector of sinusoidal data.
//It then ouputs that data along with filter information to the serial terminal. This can be read and saved to a file for later analysis. 
//If you are interested in making your own input data, simply make a vector in matlab and view the format for saving in "test.h"
//Updated 1/25/2021
//J.D. Peiffer

#include "buffer.hpp"       
#include "20_110filter.h"     //filter coeficcients
#include "test_SNR4.h"             //test dataset. instead of reading from ADC, just read from this vector
#include "test_SNR8.h"   
#include "test_SNR12.h"   
#include "test_SNR16.h"   

//For interrupts
volatile unsigned long currentRead;
volatile unsigned long computeFlag;
unsigned long currentReadCopy;
unsigned long computeFlagCopy;
IntervalTimer interruptTimer;

#define SAMPLING_RATE 500   //for 2kHz or 0.0005 s
#define thresh 100 //TRIGGERING THRESHOLD! Range of 0-512

//Prototypes
void readADC(void);
int getMaxAmplitude(double* amplitudes);

//Data locations that filters will continously update
int phaseDetect[4] = {0, 0, 0, 0};            //keeps track of what phase each filter is at
double amplitudes[4] = {0.0, 0.0, 0.0, 0.0};  //keeps track of what amplitude each filter is at

//Initializes filters
jd::buffer band_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect[0]);
jd::buffer band_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect[1]);
jd::buffer band_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect[2]);
jd::buffer band_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect[3]);

void setup() {
  Serial.begin(9600);
  delay(5000);
  while (!Serial);
  interruptTimer.begin(readADC, SAMPLING_RATE*10);  //slowed down for putty


  //Each line of the serial output will read: [X (input vector), 35 Hz centered phase filter, 35 Hz centered amplitude filter, ...and so on for 55, 75, 95, Filter bank with strongest signal, Detected phase]
  Serial.println("X,35Phase,35Amplitude,55Phase,55Amplitude,75Phase,75Amplitude,95Phase,95Amplitude,CurrentBest,PhaseDetect");

}
//==========
// Main Loop
//==========
int* x=test_SNR4;
int counter=0;
int currentbest = 0;
int SNR=4;
void loop() {
  //gets ADC (or read from vector at 2 kHz)
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

    //Outputs data, can read this into data file with PuTTy, in real life, this is where we will trigger
    Serial.print(currentReadCopy);
    Serial.print(',');
    band_20_50.plt(0);
    band_40_70.plt(0);
    band_60_90.plt(0);
    band_80_110.plt(0);
    currentbest = getMaxAmplitude(amplitudes);
    Serial.print(currentbest);
    Serial.print(',');
    Serial.print(phaseDetect[currentbest]);
    Serial.print(',');
    Serial.print(amplitudes[0]);
    Serial.print(',');
    Serial.print(amplitudes[1]);
    Serial.print(',');
    Serial.print(amplitudes[2]);
    Serial.print(',');
    Serial.print(amplitudes[3]);
    Serial.print(',');
    Serial.println(SNR);

  } 
  //only iterates through vector once
  if(counter>=LEN){
    switch (SNR){
      case 4:
        SNR=8;
        x=test_SNR8;
      break;
      case 8:
        SNR=12;
        x=test_SNR12;
      break;
      case 12:
        SNR=16;
        x=test_SNR16;
      break;
      case 16:
        while(1){
          delay(100);
        }
      break;
    }
    counter=0;
  }
}

//=============
//End Main Loop
//=============



//=========
//Functions
//=========

//decides which bank has the highest amplitude
int getMaxAmplitude(double* amplitudes) {
  double maxx = 0.0;
  int maxind=0;
  for (int i = 0; i < 4; i++) {
    if (amplitudes[i] > maxx) {
      maxx = amplitudes[i];
      maxind = i;
    }
  }
  return maxind;
}

//this runs every interrupt, normally (above) we would read from ADC, but since we are using generated data, just read from x
void readADC(void) {
  currentRead = x[counter];
  computeFlag = 1;
  counter++;
  
}

//=============
//End Functions
//=============
