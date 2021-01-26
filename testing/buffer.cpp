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
  //pushes the index and if it goes too far, loops back

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

  //filters
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

  //if peak or trough
  if (yPhCurrent - 1 < yPh)
  {
    yPh_L1 = yPhEnd;
    yPh_L2 = yPh_L1 - 1;
    yAmp_L1 = yAmpEnd;
  }
  else if (yPhCurrent - 1 == yPh)
  {
    yPh_L1 = yPh;
    yPh_L2 = yPhEnd;
    yAmp_L1 = yAmp;
  }
  else
  {
    yPh_L1 = yPhCurrent - 1;
    yPh_L2 = yPh_L1 - 1;
    yAmp_L1 = yAmpCurrent - 1;
  }

  //peak in phase(1 sample delay)
  if (*yPh_L1 > *yPh_L2 && *yPh_L1 > *yPhCurrent) {
    peakAmplitude = *yAmp_L1;
    if (*yPh_L1 > thresh)
    {
      *stat = 2;
      
    }
  }
  //trough in phase (1 sample delay)
  else if (*yPh_L1 < *yPh_L2 && *yPh_L1 < *yPhCurrent) {
    peakAmplitude = *yAmp_L1*(-1.0);
    if (*yPh_L1 < (-1.0) * thresh) //could maybe speed that up
    {
      *stat = -2;
      
    }
  }
   //+- 0 cross
  else if (*yPhCurrent < 0.0 && *yPh_L1 > 0.0) {
    *stat = -1;
  }
  //-+ 0 cross
  else if (*yPhCurrent > 0.0 && *yPh_L1 < 0.0) {
    *stat = 1;
  } else {
    *stat = 0;
  }
  return peakAmplitude;
}




buffer_low_band::buffer_low_band(double *APh, double *BPh, uint PHORDER, double *AAmp, double *BAmp, uint AMPORDER, double threshol, int *STAT) : aPh(APh), bPh(BPh), aAmp(AAmp), bAmp(BAmp), PhSize(PHORDER + 1), AmpSize(AMPORDER + 1), thresh(threshol), stat(STAT)
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

}


void buffer_low_band::plt(double Yadd, int End)
{
  Serial.print(*yPhCurrent + Yadd, 6);
  Serial.print(',');
  Serial.println(*yAmpCurrent + Yadd, 6);
}

void buffer_low_band::plt(double Yadd)
{
  Serial.print(*yPhCurrent + Yadd, 6);
  Serial.print(',');
  Serial.print(*yAmpCurrent + Yadd, 6);
  Serial.print(',');
}

double buffer_low_band::insert(double in)
{
  //pushes the index and if it goes too far, loops back

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

  //filters
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

  //if peak or trough
  //THIS COULD BE OPTIMIZED AS WELL
  if (yPhCurrent - 1 < yPh)
  {
    yPh_L1 = yPhEnd;
    yPh_L2 = yPh_L1 - 1;
    yPh_L3 = yPh_L2 - 1;
    yPh_L4 = yPh_L3 - 1;
    yAmp_L2 = yAmpEnd - 1;
  }
  else if (yPhCurrent - 1 == yPh)
  {
    yPh_L1 = yPh;
    yPh_L2 = yPhEnd;
    yPh_L3 = yPh_L2 - 1;
    yPh_L4 = yPh_L3 - 1;
    yAmp_L2 = yAmpEnd;
  }
  else if (yPhCurrent - 2 == yPh)
  {
    yPh_L2 = yPh;
    yPh_L3 = yPhEnd;
    yPh_L1 = yPh_L2 + 1;
    yPh_L4 = yPh_L3 - 1;
    yAmp_L2 = yAmp;
  }
  else if (yPhCurrent - 3 == yPh)
  {
    yPh_L1 = yPhCurrent-1;
    yPh_L2 = yPh_L1 - 1;
    yPh_L3 = yPh_L2 - 1;
    yPh_L4 = yPhEnd;
    yAmp_L2 = yAmp + 1;
  }
  else
  {
    yPh_L1 = yPhCurrent - 1;
    yPh_L2 = yPh_L1 - 1;
    yPh_L3 = yPh_L2 - 1;
    yPh_L4 = yPh_L3 - 1;
    yAmp_L2 = yAmp + 2;
  }

  //peak in phase(2 sample delay)
  if (*yPh_L2 > *yPh_L3 && *yPh_L3 > *yPh_L4 && *yPh_L2 > *yPh_L1 && *yPh_L1 > *yPhCurrent) {
    peakAmplitude = *yAmp_L2;
    if (*yPh_L2 > thresh)
    {
      *stat = 2;
      
    }
  }
  //trough in phase (2 sample delay)
  else if (*yPh_L2 < *yPh_L3 && *yPh_L3 < *yPh_L4 && *yPh_L2 < *yPh_L1 && *yPh_L1 < *yPhCurrent) {
    peakAmplitude = *yAmp_L2*(-1.0);
    if (*yPh_L2 < (-1.0) * thresh) //could maybe speed that up
    {
      *stat = -2;
      
    }
  }
   //+- 0 cross (2 sample delay)
  else if (*yPhCurrent < 0.0 && *yPh_L1 < 0.0 && *yPh_L3 > 0.0 && *yPh_L4 > 0.0) {
    *stat = -1;
  }
  //-+ 0 cross (2 sample delay)
  else if (*yPhCurrent > 0.0 && *yPh_L1 > 0.0 && *yPh_L3 < 0.0 && *yPh_L4 < 0.0) {
    *stat = 1;
  } else {
    *stat = 0;
  }
  return peakAmplitude;
}


}; // namespace jd
