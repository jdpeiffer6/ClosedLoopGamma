function [voltages] = calc_with_w(r,c,w)
%% DC
v_dc=5.*r(3)./(r(2)+r(3));

%% AC
Xc=(sqrt(-1).*2.*pi.*w.*1e-6).^-1;
v_ac_peak=abs(3.3.*paradd(r(2),r(3))./(r(1)+Xc+paradd(r(2),r(3))));
v_ac_trough=-3.3.*abs(paradd(r(2),r(3))./(r(1)+Xc+paradd(r(2),r(3))));

%% Add DC and AC
voltages{1}=v_dc+v_ac_peak;   %-3.3 because we minimizing
voltages{2}=v_dc+v_ac_trough; % for op-amp
 
end

