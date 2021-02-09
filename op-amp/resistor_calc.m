function output_voltage=resistor_calc(r)
v1=3.3;
v2=5;
vo=3.3;
output_voltage(1)=(v1/r(1)-vo/r(1))+(v2/r(2)-vo/r(2));

v1=-3.3;
v2=5;
vo=0;
output_voltage(2)=(v1/r(1)-vo/r(1))+(v2/r(2)-vo/r(2));
output_voltage=output_voltage.*1000;
end