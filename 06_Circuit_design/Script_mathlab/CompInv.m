Vdd = 3.3;
Vref = 3.3;
VinH = 4.2;
VinL = 3.8;

NTC = 0;
if(NTC)
    R25 = 100e3;
    Temp_H = 45;
    RT_L = interp1(Temp,Rnom,Temp_H);
    VinL = Vref*RT_L/(RT_L+R25)

    Temp_L = 40;
    RT_H = interp1(Temp,Rnom,Temp_L);
    VinH = Vref*RT_H/(RT_H+R25)
end


x = VinL*Vdd/(VinL*Vref+Vref*Vdd-VinH*Vref)
y = Vref*Vdd/(VinH*Vref+VinL*Vdd-VinL*Vref)

if(y>1)
    y = 1;
    z = y*VinL/(x*Vref);
    R3 = 1e6;
    R1 = (1-z)*R3/(x*z);
    R2 = R1*x/(1-x);

    VinLC = x*z*Vref/y
    VinHC = (x*z*Vref+(1-z)*Vdd)/y


    R1N = NormaliseComp(R1, 96);
    R2N = NormaliseComp(R2, 96);

    xN = R2N/(R1N+R2N);
    zN = R3/(xN*R1N+R3);
    yN = 1;

    VinLN = xN*zN*Vref/yN
    VinHN = (xN*zN*Vref+(1-zN)*Vdd)/yN

    VrefT = Vref*[1.002 1 1 1 1];
    VddT = Vdd*[1 1.02 1 1 1];
    R1T = R1*[1 1 1.001 1 1];
    R2T = R2*[1 1 1 1.001 1];
    R3T = R3*[1 1 1 1 1.01];

    xT = R2T./(R1T+R2T);
    zT = R3T./(xT.*R1T+R3T);
    yT = 1;
    VinLT = xT.*zT.*VrefT./yT;
    VinHT = (xT.*zT.*VrefT+(1-zT).*VddT)./yT;

    errH = abs(VinHT - VinHC)*1000
    errL = abs(VinLT - VinLC)*1000





else
    x = 1;
    z = y*VinL/(x*Vref);
    R3 = 1e6;
    R1 = (1-z)*R3/(x*z);
    R4 = 1e6;
    R5 = R4*y/(1-y);

    VinLC = x*z*Vref/y
    VinHC = (x*z*Vref+(1-z)*Vdd)/y

    R1N = NormaliseComp(R1, 96);
    R5N = NormaliseComp(R5, 96);

    xN = 1;
    zN = R3/(xN*R1N+R3);
    yN = R5N/(R4+R5N);

    VinLN = xN*zN*Vref/yN
    VinHN = (xN*zN*Vref+(1-zN)*Vdd)/yN

    VrefT = Vref*[1.002 1 1 1 1 1];
    VddT = Vdd*[1 1.02 1 1 1 1];
    R1T = R1*[1 1 1.01 1 1 1];
    R3T = R3*[1 1 1 1.01 1 1];
    R4T = R4*[1 1 1 1 1.01 1];
    R5T = R5*[1 1 1 1 1 1.01];

    xT = 1;
    zT = R3T./(xT.*R1T+R3T);
    yT = R5T/(R4+R5T);
    VinLT = xT.*zT.*VrefT./yT;
    VinHT = (xT.*zT.*VrefT+(1-zT).*VddT)./yT;

    errH = abs(VinHT - VinHC)*1000
    errL = abs(VinLT - VinLC)*1000



end

errH_TOT = sum(errH)
errL_TOT = sum(errL)

VinH_Max = VinHN+errH_TOT/1000
VinH_Min = VinHN-errH_TOT/1000

VinL_Max = VinLN+errL_TOT/1000
VinL_Min = VinLN-errL_TOT/1000


if(NTC)
    RTinH_max = R25*VinH_Max/(Vref-VinH_Max);
    TempL_max_max = interp1(Rmax,Temp,RTinH_max);
    TempL_max_min = interp1(Rmin,Temp,RTinH_max);

    RTinH_min = R25*VinH_Min/(Vref-VinH_Min);
    TempL_min_max = interp1(Rmax,Temp,RTinH_min);
    TempL_min_min = interp1(Rmin,Temp,RTinH_min);

    TempL = [TempL_max_max TempL_max_min TempL_min_max TempL_min_min]

    RTinL_max = R25*VinL_Max/(Vref-VinL_Max);
    TempH_max_max = interp1(Rmax,Temp,RTinL_max);
    TempH_max_min = interp1(Rmin,Temp,RTinL_max);

    RTinL_min = R25*VinL_Min/(Vref-VinL_Min);
    TempH_min_max = interp1(Rmax,Temp,RTinL_min);
    TempH_min_min = interp1(Rmin,Temp,RTinL_min);

    TempH = [TempH_max_max TempH_max_min TempH_min_max TempH_min_min]
end