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