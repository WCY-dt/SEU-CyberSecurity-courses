clc;clear;close all;
%BPSK 
M_bpsk=2;
k_2psk=log2(M_bpsk);
n=30000;
rng default
dataIn = randi([0 1],n,1); % 产生0和1之间的整数，输出一个n*1的向量
dataInMatrix_BPSK = reshape(dataIn,length(dataIn)/1,1);
dataSymbolsIn_BPSK = bi2de(dataInMatrix_BPSK);%将每行转化为十进制数
dataMod_BPSK = pskmod(dataSymbolsIn_BPSK,M_bpsk);
fprintf('\n This is BPSK experiment.\n ')
for snr=0:30
    receivedSignal_BPSK = awgn(dataMod_BPSK,snr,'measured');  
    dataSymbolsOut_BPSK = pskdemod(receivedSignal_BPSK,M_bpsk);
    dataOutMatrix_BPSK = de2bi(dataSymbolsOut_BPSK,k_2psk);
    dataOut_BPSK = dataOutMatrix_BPSK(:); % Return data in column vector
    [numErrors,ber] = biterr(dataIn,dataOut_BPSK);
    fprintf('\n The SNR is %d dB.\n ',snr)
    fprintf('\nThe binary coding bit error rate is %5.2e, based on %d errors.\n', ...
    ber,numErrors)
end
%QAM
for i=2:6
M = 2^i;%调制顺序(信号星座点数)
k = log2(M);%每符号的比特位数
n = 30000;%处理的比特总数
sps = 1;%每个符号的样本数(过采样因子)
rng default;%每次都产生相同的随机数
dataIn = randi([0 1],n,1); % 产生0和1之间的整数，输出一个n*1的向量
dataInMatrix = reshape(dataIn,length(dataIn)/k,k);
% 重组dataIn，使其每行只有4列
dataSymbolsIn = bi2de(dataInMatrix);%将每行转化为十进制数
dataMod = qammod(dataSymbolsIn,M); 
fprintf('\n This is %d-QAM experiment.\n ',M)
for snr=0:30
    %snr = Ebno+10*log10(k)-10*log10(sps);
    receivedSignal = awgn(dataMod,snr,'measured');
    sPlotFig = scatterplot(receivedSignal,1,0,'g.');
    hold on
scatterplot(dataMod,1,0,'k*',sPlotFig)
dataSymbolsOut = qamdemod(receivedSignal,M);
dataOutMatrix = de2bi(dataSymbolsOut,k);
dataOut = dataOutMatrix(:); % Return data in column vector
[numErrors,ber] = biterr(dataIn,dataOut);
%该函数比较两种数据，并返回不同的个数以及占比
fprintf('\n The SNR is %d dB.\n ',snr)
fprintf('\nThe binary coding bit error rate is %5.2e, based on %d errors.\n', ...
    ber,numErrors)
end
end