clear
%% Output Constants
x_no=1;

b35p=2;
b35a=3;

b55p=4;
b55a=5;

b75p=6;
b75a=7;

b95p=8;
b95a=9;
best_no=10;
detection_no=11;
amp_data=12:15;
trial_no=16;

%% Read and organize data

%Generated Signals
load('X.mat');

%Teensy signals
data=readmatrix('putty_new.log');
t=(0:1/2000:X.time_len)';

% x=data(:,x_no);
% t=t(1:length(x));
% clean=clean(1:length(x));
% test=test(1:length(x));
% test=transpose(test);
% band35.phase=data(:,b35p);
% band35.amplitude=data(:,b35a);
% band55.phase=data(:,b55p);
% band55.amplitude=data(:,b55a);
% band75.phase=data(:,b75p);
% band75.amplitude=data(:,b75a);
% band95.phase=data(:,b95p);
% band95.amplitude=data(:,b95a);
% best=data(:,best_no);
% detection=data(:,detection_no);


