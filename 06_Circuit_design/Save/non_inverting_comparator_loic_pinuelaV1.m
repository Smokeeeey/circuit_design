clc;
clear;
close all;

%% 1) Données
Vin = 5;        % source
VDD  = 3.3;     
Vref = 2.5;     % source de référence

VinH = 4.2;             
VinL = 2.8;           

R3 = 1e6;
R4 = 1e6;


%% 2) Calculs 
x = (Vref * VDD) / ((Vref*VinL) + (VDD * VinH) - (VinH * Vref));
y = (VinH * VDD) / ((Vref*VinH) - (Vref * VinL) + (Vref * VDD));

z = (y / x) * (Vref / VinH);

if x >= 1
    x = 1;
    R1 = 0;
    R2 = 0;
else
    R1 = R3 * ((1 - z) / (x * z));
    R2 = R1 * (x / (1 - x));
end

if y >= 1
    y = 1;
    R4 = 0;
    R5 = 0;
else
    R5 = R4 * (y / (1 - y));
end

%% Normalisation
R1_n = NormaliseComp(R1, 24);
R2_n = NormaliseComp(R2, 24);
R3_n = NormaliseComp(R3, 24);
R4_n = NormaliseComp(R4, 24);
R5_n = NormaliseComp(R5, 24);

x_n = R2_n / (R1_n + R2_n);

if (R4_n + R5_n) ~= 0
    y_n = R5_n / (R4_n + R5_n);
else
    y_n = 1; 
end

z_n = R3_n / (R1_n * x_n + R3_n);

VinH_n = (y_n * Vref) / (x_n * z_n);
VinL_n = (y_n * Vref - (1 - z_n) * VDD) / (x_n * z_n);

%% Tolérences
R1_t = R1_n     *  [1.01 1 1 1 1 1 1];
R2_t = R2_n     *  [1 1.01 1 1 1 1 1];
R3_t = R3_n     *  [1 1 1.01 1 1 1 1];
R4_t = R4_n     *  [1 1 1 1.01 1 1 1];
R5_t = R5_n     *  [1 1 1 1 1.01 1 1];
Vref_t = Vref *  [1 1 1 1 1 1.01 1];
VDD_t =  VDD  *  [1 1 1 1 1 1 1.01];

x_t = R2_t ./ (R1_t + R2_t);


% y dépend de R4/R5 : si branche supprimée -> y=1
y_t = ones(size(x_t));
idx = (R4_t + R5_t) ~= 0;
y_t(idx) = R5_t(idx) ./ (R4_t(idx) + R5_t(idx));

z_t = R3_t ./ (x_t .* R1_t + R3_t);

VinH_t = y_t .* Vref_t ./ (x_t .* z_t);
VinL_t = (y_t .* Vref_t - (1 - z_t) .* VDD_t) ./ (x_t .* z_t);

errH = abs(VinH_t - VinH) * 1000;
errL = abs(VinL_t - VinL) * 1000;

VinH_min = min(VinH_t);  VinH_max = max(VinH_t);
VinL_min = min(VinL_t);  VinL_max = max(VinL_t);

%% Résultats
fprintf('\n===== RESULTATS CALCUL =====\n');

fprintf('\n--- Coefficients ---\n');
fprintf('x = %.5f\n', x);
fprintf('y = %.5f\n', y);
fprintf('z = %.5f\n', z);

fprintf('\n--- Coefficients normalisés---\n');
fprintf('x_n = %.5f\n', x_n);
fprintf('y_n = %.5f\n', y_n);
fprintf('z_n = %.5f\n', z_n);

fprintf('\n--- Resistances ---\n');
fprintf('R1 = %.3f kOhm\n', R1/1e3);
fprintf('R2 = %.3f kOhm\n', R2/1e3);
fprintf('R3 = %.3f kOhm\n', R3/1e3);
fprintf('R4 = %.3f kOhm\n', R4/1e3);
fprintf('R5 = %.3f kOhm\n', R5/1e3);

fprintf('\n--- Resistances normalisees (E24) ---\n');
fprintf('R1_n = %.3f kOhm\n', R1_n/1e3);
fprintf('R2_n = %.3f kOhm\n', R2_n/1e3);
fprintf('R3_n = %.3f kOhm\n', R3_n/1e3);
fprintf('R4_n = %.3f kOhm\n', R4_n/1e3);
fprintf('R5_n = %.3f kOhm\n', R5_n/1e3);

fprintf('\n--- Vin et Vin_n ---\n');
fprintf('VinH = %.5f\n', VinH);
fprintf('VinL = %.5f\n', VinL);
fprintf('VinH_n = %.5f\n', VinH_n);
fprintf('VinL_n = %.5f\n', VinL_n);


names = {'R1','R2','R3','R4','R5','Vref','VDD'};

Tol_results = [VinH_t' errH' VinL_t' errL']

fprintf('\n=============================\n\n');




