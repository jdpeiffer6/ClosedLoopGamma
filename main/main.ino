#include "buffer.hpp"       
#include "20_110filter.h"     //filter coeficcients


//For interrupts
volatile unsigned long currentRead;
volatile unsigned long computeFlag;
unsigned long currentReadCopy;
unsigned long computeFlagCopy;
IntervalTimer interruptTimer;

#define ADC_PIN A7
#define SAMPLING_RATE 500   //for 2kHz or 0.0005 s
#define thresh 100 //Remember, the max is 514 here!

//prototypes
void readADC(void);
int getMaxAmplitude(double* amplitudes);



int phaseDetect[4] = {0, 0, 0, 0};            //keeps track of what phase each filter is at
double amplitudes[4] = {0.0, 0.0, 0.0, 0.0};  //keeps track of what amplitude each filter is at

//initializes filters
jd::buffer band_20_50(a_20_50_2, b_20_50_2, 2, a_20_50_4, b_20_50_4, 4, thresh, &phaseDetect[0]);
jd::buffer band_40_70(a_40_70_2, b_40_70_2, 2, a_40_70_4, b_40_70_4, 4, thresh, &phaseDetect[1]);
jd::buffer band_60_90(a_60_90_2, b_60_90_2, 2, a_60_90_4, b_60_90_4, 4, thresh, &phaseDetect[2]);
jd::buffer band_80_110(a_80_110_2, b_80_110_2, 2, a_80_110_4, b_80_110_4, 4, thresh, &phaseDetect[3]);

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
int i=0;
int currentbest = 0;
void loop() {
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

void readADC(void) {
  currentRead = analogRead(ADC_PIN);
  computeFlag = 1;
}
