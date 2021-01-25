at low SNR there is a lot of false peak/trough/zero-crossing detection in the low frequency band. This is probably because we are using adjacent samples to estimate these phases of the cycle, and for slow oscillations the signal change from sample to sample will be a lot less than for higher frequencies. Consequently, the noise will dominate over it.

 
--To circumvent this, perhaps we could increase delay between samples being compared to estimate zero-crossings and peaks/troughs? Instead of waiting one sample, it might make sense to wait 2-3 samples in the 20-50 bank. 

--In addition, sometimes it appears that multiple events are detected in close proximity to each other (within a few milliseconds). To deal with this, we might want to impose a delay after an event is detected that no other event can occur for another 4 samples. Both changes would only be applied for the 20-50 Hz bank, which shows this problem the most.
