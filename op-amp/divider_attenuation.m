
%% Using a 1uF cap
w=20:110;
r_with_1uf=[5.230112892048442e+03,1.539400900018630e+04,7.930247060701302e+03];
c=1e-6;
out1=calc_with_w(r_with_1uf,c,w);
subplot(2,1,1);
plot(w,out1{1},'LineWidth',1);
ylabel('Voltage at peak (3.3 V)');

subplot(2,1,2);
plot(w,out1{2},'LineWidth',1);
xlabel('Frequency (Hz)');
ylabel('Voltage at trough (-3.3 V)');
sgtitle('Attenuation due to 1uF capicator');

%% Using a 1mF cap
figure;
r_with_1mF=[5.475902721836059e+03,1.515820914271500e+04,7.808774406852699e+03];
c=1e-3;
out2=calc_with_w(r_with_1mF,c,w);
subplot(2,1,1);
plot(w,out2{1},'LineWidth',1);
ylabel('Voltage at peak (3.3 V)');

subplot(2,1,2);
plot(w,out2{2},'LineWidth',1);
xlabel('Frequency (Hz)');
ylabel('Voltage at trough (-3.3 V)');
sgtitle('Attenuation due to 1mF capicator');

%% Using a 0.5mF cap
figure;
r_with_05uF=[4.515619640516988e+03,1.615190950994725e+04,8.320680656638031e+03];
c=0.5e-3;
out3=calc_with_w(r_with_05uF,c,w);
subplot(2,1,1);
plot(w,out3{1},'LineWidth',1);
ylabel('Voltage at peak (3.3 V)');

subplot(2,1,2);
plot(w,out3{2},'LineWidth',1);
xlabel('Frequency (Hz)');
ylabel('Voltage at trough (-3.3 V)');
sgtitle('Attenuation due to 0.5uF capicator');