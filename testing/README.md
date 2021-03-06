# Fixes
### Double Detection
Sometimes multiple events are detected in close proximity to each other.
![Double detect](./.images/before_counter.png)

**Solution 1**: Add a delay of samples in which another POI is prohibited from triggering.  
This works O.K but since the frequency of the signals varies so widely, even between banks, it is hard to catch them all.  
![Solution 1](./.images/after_counter.png)  
**Solution 2**: Dictate that POI must occur in a certain order, for instance peak, +/- cross, trough, -/+ zero cross and reject any detection out of that order.  
![Solution 2](./.images/after_solution2.png)
### False Detection
Sometimes detected peaks are not the real peaks!
![False detection Sol](./.images/false_detection_sol.png)

**Solution 1**: Instead of a 1 sample delay in detection, increase the lag to get more data points on either side of the peak/trough. Instead of 1 sample before and 1 sample after being compared to the peak, we could have 2 before and 2 after.  

**Solution 2**: Increase how many samples _after_ the peak/trough we keep track of. Currently only have 1 after and 1 before. This could increase to 3 after and 1 before!

### Band Switching
![Band switching issue](./.images/band_switching_issue.png)  
**Solution 1**: Require a band to be the highest band for 50 samples before it switches. Another idea could be to have this length be different for different bands since they are different frequencies, but I am not sure that is necessary at this time.  
![Band switching Solution](./.images/bank_switch_sol.png)  
The image below shows the band switch being limited until bank 2 has held the highest amplitude for 50 samples. If during the time bank 2 is acculumating these 50 samples, a different bank holds the highest amplitude, then bank 2 must restart its counting.  
![Band switching Solution](./.images/bank_switch.png)  

