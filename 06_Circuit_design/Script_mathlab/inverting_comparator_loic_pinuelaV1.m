clc;
clear;
close all;

%% 1) Données
VDD  = 3.3;     
Vref = 3.3;   
VinH = 4.2;             
VinL = 3.8;           

%% 2) Calculs (comparateur inverseur)
x = (VinL * VDD) / ((VinL * Vref) + (Vref * VDD) - (VinH * Vref));
y = (Vref * VDD) / ((VinH * Vref) + (VinL * VDD) - (VinL * Vref));

if y > 1
    y = 1;
    z = y * VinL / (x * Vref);

    R3 = 1e6;
    R1 = (1 - z) * R3 / (x * z);
    R2 = R1 * x / (1 - x);
    R4 = 0;
    R5 = 0;

    % Valeurs théoriques
    VinLC = x * z * Vref / y;
    VinHC = (x * z * Vref + (1 - z) * VDD) / y;

    % Normalisation
    R1_n = NormaliseComp(R1, 96);
    R2_n = NormaliseComp(R2, 96);
    R3_n = R3;
    R4_n = 0;
    R5_n = 0;

    x_n = R2_n / (R1_n + R2_n);
    z_n = R3_n / (x_n * R1_n + R3_n);
    y_n = 1;

    VinL_n = x_n * z_n * Vref / y_n;
    VinH_n = (x_n * z_n * Vref + (1 - z_n) * VDD) / y_n;

    % Tolérances
    Vref_t = Vref * [1.002 1 1 1 1];
    VDD_t  = VDD  * [1 1.02 1 1 1];
    R1_t   = R1_n * [1 1 1.001 1 1];
    R2_t   = R2_n * [1 1 1 1.001 1];
    R3_t   = R3_n * [1 1 1 1 1.01];

    x_t = R2_t ./ (R1_t + R2_t);
    z_t = R3_t ./ (x_t .* R1_t + R3_t);
    y_t = 1;

    VinL_t = x_t .* z_t .* Vref_t ./ y_t;
    VinH_t = (x_t .* z_t .* Vref_t + (1 - z_t) .* VDD_t) ./ y_t;

    errH = abs(VinH_t - VinHC) * 1000;
    errL = abs(VinL_t - VinLC) * 1000;

else
    x = 1;
    z = y * VinL / (x * Vref);

    R3 = 1e6;
    R1 = (1 - z) * R3 / (x * z);
    R4 = 1e6;
    R5 = R4 * y / (1 - y);
    R2 = 0;

    % Valeurs théoriques
    VinLC = x * z * Vref / y;
    VinHC = (x * z * Vref + (1 - z) * VDD) / y;

    % Normalisation
    R1_n = NormaliseComp(R1, 96);
    R5_n = NormaliseComp(R5, 96);
    R3_n = R3;
    R4_n = R4;
    R2_n = 0;

    x_n = 1;
    z_n = R3_n / (x_n * R1_n + R3_n);
    y_n = R5_n / (R4_n + R5_n);

    VinL_n = x_n * z_n * Vref / y_n;
    VinH_n = (x_n * z_n * Vref + (1 - z_n) * VDD) / y_n;

    % Tolérances
    Vref_t = Vref * [1.002 1 1 1 1 1];
    VDD_t  = VDD  * [1 1.02 1 1 1 1];
    R1_t   = R1_n * [1 1 1.01 1 1 1];
    R3_t   = R3_n * [1 1 1 1.01 1 1];
    R4_t   = R4_n * [1 1 1 1 1.01 1];
    R5_t   = R5_n * [1 1 1 1 1 1.01];

    x_t = 1;
    z_t = R3_t ./ (x_t .* R1_t + R3_t);
    y_t = R5_t ./ (R4_n + R5_t);

    VinL_t = x_t .* z_t .* Vref_t ./ y_t;
    VinH_t = (x_t .* z_t .* Vref_t + (1 - z_t) .* VDD_t) ./ y_t;

    errH = abs(VinH_t - VinHC) * 1000;
    errL = abs(VinL_t - VinLC) * 1000;
end

errH_TOT = sum(errH);
errL_TOT = sum(errL);

VinH_Max = VinH_n + errH_TOT / 1000;
VinH_Min = VinH_n - errH_TOT / 1000;
VinL_Max = VinL_n + errL_TOT / 1000;
VinL_Min = VinL_n - errL_TOT / 1000;

%% Résultats
fprintf('\n===== RESULTATS CALCUL (INVERSEUR) =====\n');

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

fprintf('\n--- Resistances normalisees (E96) ---\n');
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

fprintf('\n--- Valeurs théoriques ---\n');
fprintf('VinHC = %.5f\n', VinHC);
fprintf('VinLC = %.5f\n', VinLC);

fprintf('\n--- Erreurs totales ---\n');
fprintf('errH_TOT = %.4f mV\n', errH_TOT);
fprintf('errL_TOT = %.4f mV\n', errL_TOT);

fprintf('\n--- Intervalles avec tolérances ---\n');
fprintf('VinH_Max = %.5f\n', VinH_Max);
fprintf('VinH_Min = %.5f\n', VinH_Min);
fprintf('VinL_Max = %.5f\n', VinL_Max);
fprintf('VinL_Min = %.5f\n', VinL_Min);

fprintf('\n=============================\n\n');