a=input('Do you want to create new test data? (y/n)\n');
if a=='y'
    time_len=10;
    f1=20;
    f2=110;
    
    %% 16 SNR
    SNR=16;
    [test_16,clean_16]=genGammaNoise(f1,f2,SNR,time_len);
    clean_16=clean_16';
    test_16=test_16';
    writeCvector('test_SNR16',test_16,'int');
    
    %% 12 SNR
    SNR=12;
    [test_12,clean_12]=genGammaNoise(f1,f2,SNR,time_len);
    clean_12=clean_12';
    test_12=test_12';
    writeCvector('test_SNR12',test_12,'int');
    
    %% 8 SNR
    SNR=8;
    [test_8,clean_8]=genGammaNoise(f1,f2,SNR,time_len);
    clean_8=clean_8';
    test_8=test_8';
    writeCvector('test_SNR8',test_8,'int');
    
    %% 4 SNR
    SNR=4;
    [test_4,clean_4]=genGammaNoise(f1,f2,SNR,time_len);
    clean_4=clean_4';
    test_4=test_4';
    writeCvector('test_SNR4',test_4,'int');
    
    %% Save Data
    X.test_16=test_16;
    X.test_12=test_12;
    X.test_8=test_8;
    X.test_4=test_4;
    
    X.clean_16=clean_16;
    X.clean_12=clean_12;
    X.clean_8=clean_8;
    X.clean_4=clean_4;
    X.time_len=time_len;
    
    save('X.mat','X')
    
end
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

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%   Pink Noise Generation with MATLAB Implementation   %
%                                                      %
% Author: M.Sc. Eng. Hristo Zhivomirov       07/30/13  %
% Modified by Ziao Chen                                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function y = pinknoise(N,alpha)

% function: y = pinknoise(N)
% N - number of samples to be returned in row vector
% y - row vector of pink (flicker) noise samples

% The function generates a sequence of pink (flicker) noise samples.
% Pink noise has equal energy in all octaves (or similar log bundles) of frequency.
% In terms of power at a constant bandwidth, pink noise falls off at 3 dB per octave.

if nargin==1
    alpha = 1;
end

% difine the length of the vector
% ensure that the M is even
if rem(N,2)
    M = N+1;
else
    M = N;
end

% generate white noise FFT
X = fft(randn(1,M));

% prepare a vector for 1/f multiplication
NumUniquePts = M/2 + 1;

% multiplicate the left half of the spectrum so the power spectral density
% is proportional to the frequency by factor 1/f^alpha, i.e. the
% amplitudes are proportional to 1/sqrt(f^alpha)
X(1:NumUniquePts) = X(1:NumUniquePts)./(1:NumUniquePts).^(alpha/2);

% prepare a right half of the spectrum - a copy of the left one,
% except the DC component and Nyquist frequency - they are unique
X(NumUniquePts+1:M) = conj(X(M/2:-1:2));

% IFFT
y = ifft(X);

% prepare output vector y
y = real(y(1:N));

% ensure unity standard deviation and zero mean value
y = reshape(zscore(y),[],1);

end


function writeCvector(name,data,type)

switch type
    case 'double'
        %% makes a double vector
        fid = fopen(sprintf('../%s.h',name),'w');
        l=length(data);
        fprintf(fid,'#define LEN %d\n',l);
        fprintf(fid,'//Vector created from matlab\n\ndouble %s[%d] = { %.15f',name,l,data(1));
        for i=2:l
            fprintf(fid,' , %.15f',data(i));
        end
        fprintf(fid,'};\n');
        fclose(fid);
    case 'int'
        %% makes an int vector
        fid = fopen(sprintf('../%s.h',name),'w');
        l=length(data);
        fprintf(fid,'#define LEN %d\n',l);
        fprintf(fid,'//Vector created from matlab\n\nint %s[%d] = { %d',name,l,data(1));
        for i=2:l
            fprintf(fid,', %d',data(i));
        end
        fprintf(fid,'};\n');
        fclose(fid);
    otherwise
        fprintf('Not a valid type\n');
end

end

