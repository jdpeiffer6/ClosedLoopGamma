%% Find R1 and R2 Relationship
v_max=4;
v_min=0.1;
v_offset=5;
mymat=[3.3 1 v_max;-3.3 1 v_min];
mymat=rref(mymat);
m=mymat(1,3);
b=mymat(2,3);

multiplier=5*m/b;

%% Choose R1
r1 = 3.3e3;
r2= multiplier * r1;

%% Find Rg and Rf relationship
multiplier2= m * (r1+r2)/r2 -1;

%% Choose Rg
rg = 10e3;
rf = multiplier2*rg;

%% Output results
fprintf('R1: %.2f\nR2: %.2f\nRg: %.2f\nRf: %.2f\n',r1,r2,rg,rf); 