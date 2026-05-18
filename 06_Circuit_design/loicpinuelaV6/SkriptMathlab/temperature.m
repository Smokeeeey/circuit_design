R25 = 100e3;
Vref = 3.3;

T_hot_on  = 45;
T_hot_off = 40;
T_cold_on = 8;
T_cold_off = 12;

RT_45 = interp1(Temp, Rnom, T_hot_on);
RT_40 = interp1(Temp, Rnom, T_hot_off);
RT_8  = interp1(Temp, Rnom, T_cold_on);
RT_12 = interp1(Temp, Rnom, T_cold_off);

V45 = Vref * RT_45 / (R25 + RT_45);
V40 = Vref * RT_40 / (R25 + RT_40);
V8  = Vref * RT_8  / (R25 + RT_8);
V12 = Vref * RT_12 / (R25 + RT_12);

fprintf('\n===== SEUILS TEMPERATURE =====\n');
fprintf('45°C -> Vtemp = %.5f V\n', V45);
fprintf('40°C -> Vtemp = %.5f V\n', V40);
fprintf('12°C -> Vtemp = %.5f V\n', V12);
fprintf('8°C  -> Vtemp = %.5f V\n', V8);