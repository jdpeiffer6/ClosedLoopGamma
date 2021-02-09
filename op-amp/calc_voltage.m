function output_voltage = calc_voltage(r)
r1=r(1);
r2=r(2);
% r3=r(3);
% r4=r(4);
v2=5;
%% Case 1
v1=3.3;
output_voltage(1)=v1*r2/(r1+r2)+v2*r1/(r1+r2)-4;
% output_voltage(1)=output_voltage(1)*(1+r4/r3)-4.5;

%% Case 2
v1=-3.3;
output_voltage(2)=(v1/r1+v2/r2)/(1/r1+1/r2)-0.1;
% output_voltage(2)=output_voltage(2)*(1+r4/r3)-0.5;
end

