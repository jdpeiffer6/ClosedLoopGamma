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