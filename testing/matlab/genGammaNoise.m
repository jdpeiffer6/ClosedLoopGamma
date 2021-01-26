function [signal,gammaWave] = genGammaNoise(f1,f2,SNR,time_length)
%genGammaNoise outputs gamma signal (frequency f) with defined SNR 
%   f:      frequency of signal (for gamma, try 55 Hz)
%   SNR:    Signal to noise ratio on a log scale
%   length: how long the signal goes for in seconds (try 0.5 or 1)

%setup
t=0:(1/2000):time_length;

%generate noise
noise=transpose(pinknoise(length(t)));  % rms(noise)=1 already

%generate signal
amp = db2mag(SNR);  % convert decibles to magnitude, same as 10^(SNR/20)

x=chirp(t,f1,time_length,f2,'linear',270);

%generate signal
gammaWave = (x./rms(x)).*amp;           %scale signal
signal_unit=gammaWave+noise;

%convert noisy signal to digitized 0-1028
a=0;
b=1028;
signal = floor((signal_unit-min(signal_unit))*(b-a)/(max(signal_unit)-min(signal_unit)) + a);
gammaWave = floor((gammaWave-min(gammaWave))*(b-a)/(max(gammaWave)-min(gammaWave)) + a);
%plot(signal)
%shg
end