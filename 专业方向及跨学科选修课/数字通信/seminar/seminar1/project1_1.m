%% Initialization

clc;
clear;

%% Definitions of Channel Delay

Channel_Tau = [0 310 710 1090 1730 2510]*1e-9; % [second]
% Channel_Tau = [0 300 8900 12900 17100 20000]*1e-9;
Channel_Gain = [0 -1 -9 -10 -15 -20]; % [dB]
% Channel_Gain = [-2.5 0 -12.8 -10.0 -25.2 -16.0];

Sampling_Rate = 20 * 1e6; % [points/second]
Sample_Length = 1 / Sampling_Rate; % [second]

Channel_Tau_Index = round(Channel_Tau / Sample_Length) + 1;

%% Definitions and calculations of distance and wavelength

Moving_Speed = 5 * 1000 / 3600; % [meter/second]
TDD_Interval = 1e-3; % [second]
Moving_Distance = TDD_Interval * Moving_Speed; % [meter]

Carrier_Frequency = 2.4 * 10^9; % [Hz]
Transmission_Frequency = 3 * 1e8; %[meter/second]
Wavelength = Transmission_Frequency / Carrier_Frequency; % [meter]

Nearby_Distance = 0.06; % [meter]

Channel_Seed = 0;

%% Calculation of CIR & CFR

%% Alice to Bob
rng(Channel_Seed, 'twister');
for n = 1:512
    Channel_CIR1(n) = 0i;
end
for n = 1:6
    phi(n) = 2 * pi * (2 * (rand - 0.5) + (Channel_Tau(n) * Transmission_Frequency) / Wavelength);
    c(n)=10^(Channel_Gain(n) / 10) * exp(-1i * phi(n));
    Channel_CIR1(Channel_Tau_Index(n)) = c(n);
end
Channel_CFR1 = abs(freqz(Channel_CIR1, 512));
x = 1:512;
line1 = plot(x, Channel_CFR1,'b');
hold on

%% Bob to Alice
rng(Channel_Seed, 'twister');
for n = 1:512
    Channel_CIR2(n) = 0i;
end
for n = 1:6
    phi(n) = 2 * pi * (2 * (rand - 0.5) + (Channel_Tau(n) * Transmission_Frequency + (Moving_Distance) * 2 * (rand - 0.5)) / Wavelength);
    c(n)=10^(Channel_Gain(n) / 10) * exp(-1i * phi(n));
    Channel_CIR2(Channel_Tau_Index(n)) = c(n);
end
Channel_CFR2 = abs(freqz(Channel_CIR2, 512));
x = 1:512;
line2 = plot(x, Channel_CFR2,'r');
hold on

%% Alice to Eve
rng(Channel_Seed, 'twister');
for n = 1:512
    Channel_CIR3(n) = 0i;
end
for n = 1:6
    phi(n) = 2 * pi * (2 * (rand - 0.5) + (Channel_Tau(n) * Transmission_Frequency) + rand * 0 + (Nearby_Distance) * 2 * (rand - 0.5) / Wavelength);
    c(n)=10^(Channel_Gain(n) / 10) * exp(-1i * phi(n));
    Channel_CIR3(Channel_Tau_Index(n)) = c(n);
end
Channel_CFR3 = abs(freqz(Channel_CIR3, 512));
x = 1:512;
line3 = plot(x, Channel_CFR3,'g');

legend([line1,line2,line3],'Alice to Bob','Bob to Alice','Alice to Eve');