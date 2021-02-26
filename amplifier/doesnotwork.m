i=1;
for v_max=3.3:0.1:4
    vv(i)=v_max;
    %% Find R1 and R2 Relationship
    %v_max=3.3;
    v_min=0.1;
    v_offset=5;
    mymat=[3.3 1 v_max;-3.3 1 v_min];
    mymat=rref(mymat);
    m=mymat(1,3);
    b=mymat(2,3);
    
    multiplier=5*m/b;
    
    %% Choose R1
    r1 = 10e3;
    r2= multiplier * r1;
    
    %% Find Rg and Rf relationship
    multiplier2= m * (r1+r2)/r2 -1;
    
    %% Choose Rg
    mm(i)=multiplier2;
    i=i+1;
end
plot(vv,mm)
xlabel('Output Voltage')
ylabel('Multiplier')