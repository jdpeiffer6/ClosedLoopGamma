#include <Arduino.h>
//Implements a storage and filterming method for buffer
namespace jd {
class buffer
{

  private:
    const double thresh;

    const uint PhSize;
    const uint AmpSize;

    double xPh[5];  //filter up to order 4, can make bigger if we need to
    double *xPhEnd;
    double *xPhCurrent;
    double *xPhItr;

    double yPh[5];
    double *yPhEnd;
    double *yPhCurrent;
    double *yPhItr;

    double xAmp[5];  //filter up to order 4, can make bigger if we need to
    double *xAmpEnd;
    double *xAmpCurrent;
    double *xAmpItr;

    double yAmp[5];
    double *yAmpEnd;
    double *yAmpCurrent;
    double *yAmpItr;

    const double *aPh;
    const double *bPh;
    const double *aAmp;
    const double *bAmp;

    //for peak detection
    double peakAmplitude;
    double* yPh_L1;
    double* yPh_L2;
    double* yAmp_L1;

    //reporting phase events
    int* stat;

  public:
    buffer(double* APh, double* BPh, uint PHORDER, double* AAmp, double* BAmp, uint AMPORDER, double threshol, int *STAT);
    void plt(double Yadd, int End);
    void plt(double Yadd);
    double insert(double);

};
};
