#include "buffer.hpp"
namespace jd
{
buffer::buffer(double *APh, double *BPh, uint PHORDER, double *AAmp, double *BAmp, uint AMPORDER, double threshol, int *STAT) : aPh(APh), bPh(BPh), aAmp(AAmp), bAmp(BAmp), PhSize(PHORDER + 1), AmpSize(AMPORDER + 1), thresh(threshol), stat(STAT)
{
  this->peakAmplitude = 0.0;
  //Initializes buffer start and ends
  this->xPhCurrent = this->xPh;            //starts at beginning of "circle"
  this->xPhCurrent--;                      //backs up one
  this->xPhEnd = &(this->xPh[4]); //puts the end at the right spot

  this->xAmpCurrent = this->xAmp;
  this->xAmpCurrent--;
  this->xAmpEnd = &(this->xAmp[4]);

  this->yPhCurrent = this->yPh;
  this->yPhCurrent--;
  this->yPhEnd = &(this->yPh[4]);

  this->yAmpCurrent = this->yAmp;
  this->yAmpCurrent--;
  this->yAmpEnd = &(this->yAmp[4]);

  for (size_t i = 0; i < PhSize; i++)
  {
    yPh[i] = xPh[i] = 0.00; //pads with 0
  }

  for (size_t i = 0; i < AmpSize; i++)
  {
    yAmp[i] = xAmp[i] = 0.00; //pads with 0
  }

  //start counter
  previous_detect = 0;

}

void buffer::plt(double Yadd, int End)
{
  Serial.print(*yPhCurrent + Yadd, 6);
  Serial.print(',');
  Serial.println(*yAmpCurrent + Yadd, 6);
}

void buffer::plt(double Yadd)
{
  Serial.print(*yPhCurrent + Yadd, 6);
  Serial.print(',');
  Serial.print(*yAmpCurrent + Yadd, 6);
  Serial.print(',');
}

double buffer::insert(double in)
{
  //===========================
  //MOVE BUFFER AND LOOP AROUND
  //===========================
  //THIS COULD BE OPTIMIZED
  if (++xPhCurrent > xPhEnd)
  {
    xPhCurrent = xPh;
  }
  if (++xAmpCurrent > xAmpEnd)
  {
    xAmpCurrent = xAmp;
  }
  if (++yPhCurrent > yPhEnd)
  {
    yPhCurrent = yPh;
  }
  if (++yAmpCurrent > yAmpEnd)
  {
    yAmpCurrent = yAmp;
  }

  //Adds new x value
  *xPhCurrent = *xAmpCurrent = in;
  *yPhCurrent = *yAmpCurrent = 0.0;

  //sets iterators
  xPhItr = xPhCurrent;
  yPhItr = yPhCurrent;
  xAmpItr = xAmpCurrent;
  yAmpItr = yAmpCurrent;

  //=============
  //IIR FILTERING
  //=============

  //Phase
  for (size_t i = 0; i < PhSize; i++)
  {
    *yPhCurrent -= (*yPhItr--) * aPh[i];
    *yPhCurrent += (*xPhItr--) * bPh[i];
    //wrap around
    if (xPhItr < xPh)
    {
      xPhItr = xPhEnd;
      yPhItr = yPhEnd;
    }
  }

  //Amplitude
  for (size_t i = 0; i < AmpSize; i++)
  {
    *yAmpCurrent -= (*yAmpItr--) * aAmp[i];
    *yAmpCurrent += (*xAmpItr--) * bAmp[i];

    if (xAmpItr < xAmp)
    {
      xAmpItr = xAmpEnd;
      yAmpItr = yAmpEnd;
    }
  }

  //=============================
  //PHASE AND AMPLITUDE DETECTION
  //=============================

  //sets 1 sample delay
  if (yPhCurrent - 1 < yPh)
  {
    yPh_L1 = yPhEnd;
    yPh_L2 = yPh_L1 - 1;
    yAmp_L1 = yAmpEnd;
    yAmp_L2 = yAmp_L1 - 1;
  }
  else if (yPhCurrent - 1 == yPh)
  {
    yPh_L1 = yPh;
    yPh_L2 = yPhEnd;
    yAmp_L1 = yAmp;
    yAmp_L2 = yAmpEnd;
  }
  else
  {
    yPh_L1 = yPhCurrent - 1;
    yPh_L2 = yPh_L1 - 1;
    yAmp_L1 = yAmpCurrent - 1;
    yAmp_L2 = yAmp_L1 - 1;
  }

  //====================
  // Amplitude Detection
  //====================
  //peak
  if (*yAmp_L1 > *yAmp_L2 && *yAmp_L1 > *yAmpCurrent && *yAmp_L1 > thresh) {
    peakAmplitude = *yAmp_L1;
  }
  //trough (1 sample delay)
  else if (*yAmp_L1 < *yAmp_L2 && *yAmp_L1 < *yAmpCurrent && *yAmp_L1 < (-1.0) * thresh) {
    peakAmplitude = *yAmp_L1 * (-1.0);

  }
  //================
  // Phase Detection
  //================

  //peak (1 sample delay)
  if (*yPh_L1 > *yPh_L2 && *yPh_L1 > *yPhCurrent && peakAmplitude > thresh && previous_detect == 0) {
    *stat = 2;
    previous_detect = 1;
  }
  //trough (1 sample delay)
  //else if (*yPh_L1 < *yPh_L2 && *yPh_L1 < *yPhCurrent && peakAmplitude > (-1.0) * thresh && previous_detect == 2) {
  else if (*yPh_L1 < *yPh_L2 && *yPh_L1 < *yPhCurrent && peakAmplitude >  thresh && previous_detect == 2) {
    *stat = -2;
    previous_detect = 3;
  }
  //+- 0 cross
  else if (*yPhCurrent < 0.0 && *yPh_L1 > 0.0 && previous_detect == 1) {
    *stat = -1;
    previous_detect = 2;
  }
  //-+ 0 cross
  else if (*yPhCurrent > 0.0 && *yPh_L1 < 0.0 && previous_detect == 3) {
    *stat = 1;
    previous_detect = 0;
  } else {
    *stat = 0;
  }

  return peakAmplitude;
}

void buffer::restart(double new_threshold) {
  this->thresh = new_threshold;
  //this->thresh = 0;
  Serial.print("Using ");
  Serial.print(this->thresh);
  Serial.println("as threshold");
  this->peakAmplitude = 0.0;
  for (size_t i = 0; i < PhSize; i++)
  {
    yPh[i] = xPh[i] = 0.00; //pads with 0
  }

  for (size_t i = 0; i < AmpSize; i++)
  {
    yAmp[i] = xAmp[i] = 0.00; //pads with 0
  }

  //start counter
  previous_detect = 0;
}

void buffer::disp(void) {
  Serial.println("Phase:");
  for (int i = 0; i < PhSize; i++) {
    Serial.println(xPh[i]);
  }
  Serial.println("Amplitude:");
  for (int i = 0; i < AmpSize; i++) {
    Serial.println(xAmp[i]);
  }
}

}; // end namespace jd
