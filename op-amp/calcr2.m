function output_voltage = calc_voltage(r1,r2)
v2=5;
%% Case 1
v1=3.3;
output_voltage(1)=v1*r2/(r1+r2)+v2*r1/(r1+r2)-v1;

%% Case 2
v1=-3.3;
output_voltage(2)=(v1/r1+v2/r2)/(1/r1+1/r2);

end

