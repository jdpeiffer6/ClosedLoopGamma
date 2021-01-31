# Gamma Detection Using Teensy 4.0/4.1
### [J.D. Peiffer](mailto:jdp6n8@mail.missouri.edu), Ziao Chen, Satish Nair, Drew Headley

## Folder information
* __testing__: contains predefined input vectors (20-110 Hz chirp) for observation and testing.
    * __matlab__: contains matlab application to visualize output of testing script. Sample data is provided. Run "visualization_tool" in matlab.
* __main__: real time code with serial interface for setting threshold and triggering phase.
## Teensy Installation Instructions
* Download [Arduino IDE](https://www.arduino.cc/en/software)
* Download [Teensy Loader/Teensyduino](https://www.pjrc.com/teensy/loader.html)
    * This is a software add-on to thae arduino software. All of the programming is done in Arduino IDE and Teensy Loader helps compile and upload code.
    * Most all Arduino functions like Serial.print() and digitalWrite() work with the teensy.

## Algorithm Description
1. With any serial connection, the algorithm will halt. Type anything to bring up the serial menu. It should look something like this:  
![Serial Menu](./.images/UI.png)
2. Type an option to change parameters. Type quit to run the algorithm.
3. Teensy reads from pin A7 (labelled 21) at a rate of 2 kHz.
4. Sample is inserted to __each__ of the four filter band's (20-50, 40-70, 60-90, 80-110 Hz) amplitude and phase filter.
5. Each band calulates its amplitude from amplitude filtered signal and phase from phase filtered signal. These numbers are all reported to the main function. 
6. The main function determines which band has the highest amplitude and reports the detected phase for that band. (A band must me the strongst band for over 50 samples, 25 ms, to be considered the "strongest". This prevents switching back and forth at the edge of bands.)
 7. If the desired phase is found in the strongest band, digital pin 2 (labelled 2) will be held at a high voltage for 10 samples (5 ms).
 8. Plug in serial USB to change triggering phase or threshold.
## I/O Pins
* __Analog pin 7__ (21 on teensy): input signal (0-3.3 V)
* __Digital pin 2__ (2 on teensy): written high for triggering 
* __Ground__ (G on teensy): ground
* [Teensy 4.1 Pinout](https://www.pjrc.com/teensy/card11a_rev2_web.pdf)
* [Teensy 4.0 Pinout](https://www.pjrc.com/teensy/card10a_rev2_web.pdf)