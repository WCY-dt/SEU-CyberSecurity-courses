%% 初始化
clc;
clear;
close all;

%% 需要修改的地方
M2 = [];
M4 = [];
M16 = [1:38];
M64 = [39:52];
SNR = 20; %仿真信噪比
N_frm = 1000; % 每种信噪比下的仿真帧数

%% 定义信道
% Definitions of Channel Delay
Channel_Tau = [0 100 200 300 500 700]; % ns
Channel_Gain = [0 -3.6 -7.2 -10.8 -18 -25.2]; % dB
Channel_Tau = Channel_Tau * 1e-9; % second
Sampling_Rate = 20 * 1e6; % Points/second. Definition of your simulation sampling rate
Sample_Length = 1 / Sampling_Rate; % second. Get the sample duration in the simulation.
Channel_Tau_Index = round (Channel_Tau / Sample_Length) + 1; % Get discrete Index, from 1st. You should get the index of 1,7,15,23,36,51 with this model

% Definitions and calculations of distance and wavelength
Moving_Speed = 5; % km/h
Moving_Speed = Moving_Speed * 1000/3600; % m/s
TDD_Interval = 1e-3; % second. Alice and Bob mutually transmit message with a time interval
Moving_Distance = Moving_Speed * TDD_Interval; % m. The position change of Alice when Bob send back the signal
Carrier_Frequency = 2400 * 1e6; % Hz
Transmission_Speed = 3 * 1e8; % m/s
Wavelength = Transmission_Speed / Carrier_Frequency; % meter
Nearby_Distance = 0.12; % meter. The distance of Eavesdropper (Eve) and Bob
Channel_Seed = 1; % Fix the randomness of phase at each multi-path for Alice and Bob. (They should have very similar phase at each multi-path, NOT random phase at each multi-path. You can choose a number. Then you can change the number and observe the result changes)

rng(Channel_Seed, 'twister');

for n = 1:512
    Channel_CIR(n) = 0i;
end

for n = 1:6
    phi(n) = 2 * pi * ((Channel_Tau(n) * Transmission_Speed) / Wavelength + 2 * (rand - 0.5));
    c(n) = 10^(Channel_Gain(n) / 10) * exp(-1i * phi(n)); % Must retain complex value
    Channel_CIR(Channel_Tau_Index(n)) = c(n);
end

%% 参数设置

N_sc = 52; %系统子载波数（不包括直流载波）、number of subcarrier
N_fft = 64; % FFT 长度
N_cp = 16; % 循环前缀长度、Cyclic prefix
N_symbo = N_fft + N_cp; % 1个完整OFDM符号长度
N_c = 53; % 包含直流载波的总的子载波数、number of carriers
P_f_inter = 6; %导频间隔
data_station = []; %导频位置
L = 7; %卷积码约束长度
tblen = 6 * L; %Viterbi译码器回溯深度
stage = 3; % m序列的阶数
ptap1 = [1 3]; % m序列的寄存器连接方式
regi1 = [1 1 1]; % m序列的寄存器初始值
L = [1 1 -1 -1 1 1 -1 1 -1 1 1 1 1 1 1 -1 -1 1 1 -1 1 -1 1 1 1 1 0 1 -1 -1 1 1 -1 1 -1 1 -1 -1 -1 -1 -1 1 1 -1 -1 1 -1 1 -1 1 1 1 1];
Nd = length(M2) + (length(M4)*2) + (length(M16)*4) + (length(M64)*6); % 每帧包含的OFDM符号数

%% 基带数据数据产生
P_data = randi([0 1], 1, N_sc * Nd * N_frm);

%% 调制
code_data = reshape(P_data, (length(M2) + (length(M4)*2) + (length(M16)*4) + (length(M64)*6)), []);

cnt = 1;

for e = M2
    M = 2;
    data_temp1 = reshape(code_data(cnt, :), log2(M), [])'; %以每组2比特进行分组，M=4
    cnt = cnt + 1;
    data_temp2 = bi2de(data_temp1); %二进制转化为十进制
    modu_data(e, :) = pskmod(data_temp2, M, pi / M); % 4PSK调制
    %modu_data(e,:)=qammod(data_temp2,M);
end

for e = M4
    M = 4;
    data_temp1 = reshape(reshape(code_data(cnt:cnt + 1, :)', [], 1), log2(M), [])';
    cnt = cnt + 2;
    data_temp2 = bi2de(data_temp1); %二进制转化为十进制
    modu_data(e, :) = pskmod(data_temp2, M, pi / M); % 4PSK调制
    %modu_data(e,:)=qammod(data_temp2,M);
end

for e = M16
    M = 16;
    data_temp1 = reshape(reshape(code_data(cnt:cnt + 3, :)', [], 1), log2(M), [])';
    cnt = cnt + 4;
    data_temp2 = bi2de(data_temp1); %二进制转化为十进制
    modu_data(e, :) = qammod(data_temp2, M); % 4PSK调制
    %modu_data(e,:)=qammod(data_temp2,M);
end

for e = M64
    M = 64;
    data_temp1 = reshape(reshape(code_data(cnt:cnt + 5, :)', [], 1), log2(M), [])';
    cnt = cnt + 6;
    data_temp2 = bi2de(data_temp1); %二进制转化为十进制
    modu_data(e, :) = qammod(data_temp2, M); % 4PSK调制
    %modu_data(e,:)=qammod(data_temp2,M);
end

%% 扩频
code = mseq(stage, ptap1, regi1, N_sc); % 扩频码的生成
code = code * 2 - 1; %将1、0变换为1、-1

spread_data = spread(modu_data, code); % 扩频
spread_data = reshape(spread_data, [], 1);

%% 插入导频
P_f = 3 + 3 * 1i; %Pilot frequency
P_f_station = [1:P_f_inter:N_fft]; %导频位置
pilot_num = length(P_f_station); %导频数量

for img = 1:N_fft %数据位置

    if mod(img, P_f_inter) ~= 1 %mod(a,b)就是求的是a除以b的余数
        data_station = [data_station, img];
    end

end

data_row = length(data_station);
data_col = ceil(length(spread_data) / data_row);

pilot_seq = ones(pilot_num, data_col) * P_f; %将导频放入矩阵
data = zeros(N_fft, data_col); %预设整个矩阵
data(P_f_station(1:end), :) = pilot_seq; %对pilot_seq按行取

if data_row * data_col > length(spread_data)
    data2 = [spread_data; zeros(data_row * data_col - length(spread_data), 1)]; %将数据矩阵补齐，补0是虚载频~
end

%% 串并转换
data_seq = reshape(data2, data_row, data_col);
data(data_station(1:end), :) = data_seq; %将导频与数据合并

%% IFFT
ifft_data = ifft(data);

%% 插入保护间隔、循环前缀
Tx_cd = [ifft_data(N_fft - N_cp + 1:end, :); ifft_data];

%% 并串转换
Tx_data = reshape(Tx_cd, [], 1);

Tx_data = [L'; L'; L'; L'; L'; L'; L'; L'; L'; L'; Tx_data];

%% 信道
Ber = zeros(1, length(SNR));
Ber2 = zeros(1, length(SNR));

for jj = 1:length(SNR)
    Tx_data_mul = filter(Channel_CIR, 1, Tx_data);
    rx_channel = awgn(Tx_data_mul, SNR(jj), 'measured'); %添加高斯白噪声

    rx_channel = rx_channel(531:end, 1);

    %% 串并转换
    Rx_data1 = reshape(rx_channel, N_fft + N_cp, []);

    %% 去掉保护间隔、循环前缀
    Rx_data2 = Rx_data1(N_cp + 1:end, :);

    %% FFT
    fft_data = fft(Rx_data2);

    %% 信道估计与插值（均衡）
    data3 = fft_data(1:N_fft, :);
    Rx_pilot = data3(P_f_station(1:end), :); %接收到的导频
    h = Rx_pilot ./ pilot_seq;
    H = interp1(P_f_station(1:end)', h, data_station(1:end)', 'linear', 'extrap');

    %% 信道校正
    data_aftereq = data3(data_station(1:end), :) ./ H;
    %% 并串转换
    data_aftereq = reshape(data_aftereq, [], 1);
    data_aftereq = data_aftereq(1:length(spread_data));
    data_aftereq = reshape(data_aftereq, N_sc, length(data_aftereq) / N_sc);

    %% 解扩
    demspread_data = despread(data_aftereq, code);

    %% 解调
    prod = N_sc * Nd * N_frm / (length(M2) + (length(M4)*2) + (length(M16)*4) + (length(M64)*6));
    De_Bit = [];

    cnt = 1;

    for e = M2
        M = 2;
        demodulation_data = pskdemod(demspread_data(e, :), M, pi / M);
        De_data1 = reshape(demodulation_data, [], 1);
        De_data2 = de2bi(De_data1, log2(M));
        tmp = reshape(De_data2', 1, []);
        De_Bit(cnt, :) = reshape(tmp, prod, [])';
        cnt = cnt + 1;
    end

    for e = M4
        M = 4;
        demodulation_data = pskdemod(demspread_data(e, :), M, pi / M);
        De_data1 = reshape(demodulation_data, [], 1);
        De_data2 = de2bi(De_data1, log2(M));
        tmp = reshape(De_data2', 1, []);
        De_Bit(cnt:cnt + 1, :) = reshape(tmp, prod, [])';
        cnt = cnt + 2;
    end

    for e = M16
        M = 16;
        demodulation_data = qamdemod(demspread_data(e, :), M);
        De_data1 = reshape(demodulation_data, [], 1);
        De_data2 = de2bi(De_data1, log2(M));
        tmp = reshape(De_data2', 1, []);
        De_Bit(cnt:cnt + 3, :) = reshape(tmp, prod, [])';
        cnt = cnt + 4;
    end
    
    for e = M64
        M = 64;
        demodulation_data = qamdemod(demspread_data(e, :), M);
        De_data1 = reshape(demodulation_data, [], 1);
        De_data2 = de2bi(De_data1, log2(M));
        tmp = reshape(De_data2', 1, []);
        De_Bit(cnt:cnt + 5, :) = reshape(tmp, prod, [])';
        cnt = cnt + 6;
    end

    De_Bit = reshape(De_Bit, 1, []);

    %% 计算误码率
    [err, Ber2(jj)] = biterr(De_Bit, P_data); %译码后的误码率

end

%figure(1);
%semilogy(SNR, Ber2, 'b-s');
%hold on;
%xlabel('SNR');
%ylabel('BER');
%title('误比特率曲线');

for i=1:length(SNR)
   fprintf('\nSNR: %d dB   BER = %5.2e\n',SNR(i),Ber2(i));
end
