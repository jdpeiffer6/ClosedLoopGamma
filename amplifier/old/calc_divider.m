function voltages = calc_divider(r)
%% DC
v_dc=5*r(3)/(r(2)+r(3));

%% AC
c=1e-6;
Xc=(sqrt(-1)*2*pi*60*c)^-1;
v_ac_peak=abs(3.3*paradd(r(2),r(3))/(r(1)+Xc+paradd(r(2),r(3))));
v_ac_trough=-3.3*abs(paradd(r(2),r(3))/(r(1)+Xc+paradd(r(2),r(3))));

%% Add DC and AC
voltages(1)=v_dc+v_ac_peak-3.3;   %-3.3 because we minimizing
voltages(2)=v_dc+v_ac_trough-0.1; % for op-amp
 
%% Some component constraints
voltages(3)=r(1)-5100;
voltages(4)=r(2)-15000;
end

