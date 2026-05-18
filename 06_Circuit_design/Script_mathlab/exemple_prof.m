Vdd = 3.3;
Vref = 5;
VinH = 4.2;
VinL = 3.8;

x = Vref*Vdd/(VinH*Vdd-Vref*VinH+Vref*VinL)
y = VinH*Vdd/(Vref*Vdd+Vref*VinH-Vref*VinL)

if(y>1)
    y = 1;
    z = y*Vref/(x*VinH);
    R3 = 1e6;
    R1 = (1-z)*R3/(x*z);
    R2 = R1*x/(1-x);


    VinHC = y*Vref/(x*z)
    VinLC = (y*Vref-(1-z)*Vdd)/(x*z)

    R1N = NormaliseComp(R1, 96);
    R2N = NormaliseComp(R2, 96);

    xN = R2N/(R1N+R2N);
    zN = R3/(xN*R1N+R3);
    yN = 1;

    VinHN = yN*Vref/(xN*zN)
    VinLN = (yN*Vref-(1-zN)*Vdd)/(xN*zN)

    VrefT = Vref*[1.002 1 1 1 1];
    VddT = Vdd*[1 1.02 1 1 1];
    R1T = R1*[1 1 1.001 1 1];
    R2T = R2*[1 1 1 1.001 1];
    R3T = R3*[1 1 1 1 1.01];

    xT = R2T./(R1T+R2T);
    zT = R3T./(xT.*R1T+R3T);
    yT = 1;    
    VinHT = yT.*VrefT./(xT.*zT);
    VinLT = (yT.*VrefT-(1-zT).*VddT)./(xT.*zT);

    errH = abs(VinHT - VinHC)*1000
    errL = abs(VinLT - VinLC)*1000



else
    x = 1;
    z = y*Vref/(x*VinH);
    R3 = 1e6;
    R1 = (1-z)*R3/(x*z);
    R4 = 1e6;
    R5 = R4*y/(1-y);

    VinH = y*Vref/(x*z)
    VinL = (y*Vref-(1-z)*Vdd)/(x*z)

    R1N = NormaliseComp(R1, 96);
    R5N = NormaliseComp(R5, 96);

    xN = 1;
    zN = R3/(xN*R1N+R3);
    yN = R5N/(R4+R5N);

    VinHN = yN*Vref/(xN*zN)
    VinLN = (yN*Vref-(1-zN)*Vdd)/(xN*zN)

    VrefT = Vref*[1.002 1 1 1 1 1];
    VddT = Vdd*[1 1.02 1 1 1 1];
    R1T = R1*[1 1 1.001 1 1 1];
    R3T = R3*[1 1 1 1.01 1 1];
    R4T = R4*[1 1 1 1 1.01 1];
    R5T = R5*[1 1 1 1 1 1.01];

    xT = 1;
    zT = R3T./(xT.*R1T+R3T);
    yT = R5T/(R4+R5T);    
    VinHT = yT.*VrefT./(xT.*zT);
    VinLT = (yT.*VrefT-(1-zT).*VddT)./(xT.*zT);

    errH = abs(VinHT - VinHC)*1000
    errL = abs(VinLT - VinLC)*1000


end
