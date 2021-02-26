function out = non_invert_summing(r)
%https://www.electronics-tutorials.ws/opamp/opamp_4.html
v1 = 5; % DC volts
%% Case 1
v2 = 3.3; % AC peaks
v_out = 4;

out(1)=(v1-v_out)/r(1)-(v2-v_out)/r(2);
% out(1)=v1/r(1)-v_out/r(1)+v2/r(2)-v_out/r(2);
%% Case 2
v2 = -3.3; % AC troughs
v_out = 0.1;

out(2)=(v1-v_out)/r(1)-(v2-v_out)/r(2);
out=out.*100;
end

