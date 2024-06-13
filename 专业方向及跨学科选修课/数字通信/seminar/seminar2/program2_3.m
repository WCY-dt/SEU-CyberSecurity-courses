%%
clear;clc;
% Generation of Bits stream
N=12000; %Number of Bits
Bits_Sequence=randi([0 1],1,N); % original bits stream
 
%%
%BPSK Modulation
BPSK_Sequence=2.*Bits_Sequence-1; % uni-polar to bi-polar
 
% Adds Gaussian Noise to BPSK signal and Calculate the Bit Error Rate
SNR_dB = [0:1:30];
BPSK_BER_list=[];
 
% Received BPSK Signals
figure('name','Rx_BPSK','Position',[800,100,1100,900]);
subplot(6,6,1);
plot(real(BPSK_Sequence),imag(BPSK_Sequence),'o');
grid on;
hold on;
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
text(1.2,0.2,'1','FontSize',12,'Interpreter','latex');
text(-1.2,0.2,'0','FontSize',12,'Interpreter','latex');
title('Original Constellation Diagram');
xlim([-2,2]);
ylim([-2,2]);
hold off;
len = length(BPSK_Sequence);
 
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_BPSK_sig = BPSK_Sequence + Noise; % Received signal
    subplot(6,6,i+1)
    plot(real(Rx_BPSK_sig(1:100)),imag(Rx_BPSK_sig(1:100)),'*');
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    % Demodulation
    De_BPSK_Bits = (real(Rx_BPSK_sig)>0); %Demodulated Sequence
    Error_bits_BPSK = Bits_Sequence - De_BPSK_Bits; %Calculate Bit Errors
    BPSK_BER_list(end+1) = sum(abs(Error_bits_BPSK))/N; % BER
end
 
%%
%QPSK AWGN
% Split the bits into two streams
Bits1 = Bits_Sequence(1:2:end); % Odd order (shape 1*5000)
Bits2 = Bits_Sequence(2:2:end); % Even order
 
% QPSK pi/4 radians constellation
qpsk_sig =(((Bits1==0).*(Bits2==0)*(exp(1i*pi/4))+(Bits1==0).*(Bits2==1)...
*(exp(3*1i*pi/4))+(Bits1==1).*(Bits2==1)*(exp(5*1i*pi/4))...
+(Bits1==1).*(Bits2==0)*(exp(7*1i*pi/4))));
 
% Adds Gaussian Noise to QPSK signal
SNR_dB = [0:1:30];
QPSK_BER_list=[];
figure('name','Rx_QPSK','Position',[800,100,1100,900]);
subplot(6,6,1)
plot(real(qpsk_sig),imag(qpsk_sig),'o');
grid on
hold on
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
text(0.75,0.75,'00','FontSize',12,'Interpreter','latex');
text(-0.75,0.75,'01','FontSize',12,'Interpreter','latex');
text(-0.75,-0.75,'11','FontSize',12,'Interpreter','latex');
text(0.75,-0.75,'10','FontSize',12,'Interpreter','latex');
title('Original Constellation Diagram');
xlim([-2,2]);
ylim([-2,2]);
hold off
len = length(qpsk_sig);
 
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_QPSK_sig = qpsk_sig + Noise; % Received signal
    subplot(6,6,i+1)
    plot(real(Rx_QPSK_sig(1:100)),imag(Rx_QPSK_sig(1:100)),'*'); % Select some of the points
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    % Demodulation
    Bits4 = (real(Rx_QPSK_sig)<0);
    Bits3 = (imag(Rx_QPSK_sig)<0);
    Demod_qpsk_bits = zeros(1,2*length(Rx_QPSK_sig)); % Demodulated bits
    Demod_qpsk_bits(1:2:end) = Bits3;
    Demod_qpsk_bits(2:2:end) = Bits4;
    Error_bits_QPSK = Bits_Sequence - Demod_qpsk_bits; %Calculate Bit Errors
    QPSK_BER_list(end+1) = sum(abs(Error_bits_QPSK))/N; % BER
end
 
%%
%16-QAM AWGN
% Split the bits into four streams
%Source
%rand() generate 1*num_bits shape matrix with number randomly from (0-1)
QAM16_Bits = reshape(Bits_Sequence,4,length(Bits_Sequence)/4);
Bits1 = QAM16_Bits(1,:);
Bits2 = QAM16_Bits(2,:);
Bits3 = QAM16_Bits(3,:);
Bits4 = QAM16_Bits(4,:);
% normalizing factor
normalizer = sqrt(1/10);	% (2*4+10*8+18*4)/16=10			
 
% bit mapping        
qam16_sig = normalizer*(1j.*(2.*Bits2-1).*(-2.*Bits3+3)+(2.*Bits1-1).*(-2.*Bits4+3));
 
% Adds Gaussian Noise to QPSK signal
SNR_dB=[0:1:30];
QAM16_BER_list=[];
figure('name','Rx_QAM16','Position',[200,100,1600,900]);
subplot(6,6,1)
plot(real(qam16_sig),imag(qam16_sig),'ro'); 
grid on
hold on
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
text(0.3562,0.3562,'1111','FontSize',12,'Interpreter','latex');
text(-0.3562,0.3562,'0111','FontSize',12,'Interpreter','latex');
text(-0.3562,-0.3562,'0011','FontSize',12,'Interpreter','latex');
text(0.3562,-0.3562,'1011','FontSize',12,'Interpreter','latex');
text(0.9987,0.9987,'1100','FontSize',12,'Interpreter','latex');
text(-0.9987,0.9987,'0100','FontSize',12,'Interpreter','latex');
text(-0.9987,-0.9987,'0000','FontSize',12,'Interpreter','latex');
text(0.9987,-0.9987,'1000','FontSize',12,'Interpreter','latex');
text(0.3562,0.9987,'1101','FontSize',12,'Interpreter','latex');
text(-0.3562,0.9987,'0101','FontSize',12,'Interpreter','latex');
text(-0.3562,-0.9987,'0001','FontSize',12,'Interpreter','latex');
text(0.3562,-0.9987,'1001','FontSize',12,'Interpreter','latex');
text(0.9987,0.3562,'1110','FontSize',12,'Interpreter','latex');
text(-0.9987,0.3562,'0110','FontSize',12,'Interpreter','latex');
text(-0.9987,-0.3562,'0010','FontSize',12,'Interpreter','latex');
text(0.9987,-0.3562,'1010','FontSize',12,'Interpreter','latex');
title('16QAM Constellation');
xlim([-2,2]);
ylim([-2,2]);
hold off
len = length(qam16_sig);
 
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_qam16_sig = qam16_sig + Noise; % Received signal
    subplot(6,6,i+1)
    plot(real(Rx_qam16_sig(1:100)),imag(Rx_qam16_sig(1:100)),'*'); % Select some of the points
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    % Demodulation
    ab = 1/sqrt(10);
    Bits6 = imag(Rx_qam16_sig)>0;
    Bits7 = (imag(Rx_qam16_sig)<2*ab) & (imag(Rx_qam16_sig)>-2*ab);
    Bits5 = real(Rx_qam16_sig)>0;
    Bits8 = (real(Rx_qam16_sig)<2*ab) & (real(Rx_qam16_sig)>-2*ab);
    % Combine into single stream
    comb = [Bits5; Bits6; Bits7; Bits8];
    Demod_qam16_bits = reshape(comb,1,4*length(comb));
    Error_bits_qam16 = Bits_Sequence - Demod_qam16_bits;	  % Calculate Bit Errors
    QAM16_BER_list(end+1) = sum(abs(Error_bits_qam16))/N;	% BER
end
 
%%
% Raylaigh Channel - BPSK
SNR_dB=[0:1:30];
figure('name','Rx_BPSK','Position',[800,100,1100,900]);
subplot(6,6,1)
plot(real(BPSK_Sequence),imag(BPSK_Sequence),'o');
grid on
hold on
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
text(1.2,0.2,'1','FontSize',12,'Interpreter','latex');
text(-1.2,0.2,'0','FontSize',12,'Interpreter','latex');
title('Original Constellation Diagram');
xlim([-2,2]);
ylim([-2,2]);
hold off
len=length(BPSK_Sequence);
R_Chann=sqrt(0.5)*(randn(1,len)+1i*randn(1,len));
BPSK_R_BER_list=[];
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_BPSK_sig = BPSK_Sequence.*R_Chann + Noise; % Received signal
    Es_BPSK_sig = Rx_BPSK_sig./R_Chann;
    subplot(6,6,i+1)
    plot(real(Es_BPSK_sig(1:100)),imag(Es_BPSK_sig(1:100)),'*');
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    % Demodulation
    De_BPSK_Bits = (real(Es_BPSK_sig)>0); %Demodulated Sequence
    Error_bits_BPSK = Bits_Sequence - De_BPSK_Bits; %Calculate Bit Errors
    BPSK_R_BER_list(end+1) = sum(abs(Error_bits_BPSK))/N; % BER
end
 
%%
% Raylaigh to QPSK signal
SNR_dB = [0:1:30];
QPSK_R_BER_list=[];
figure('name','Rx_QPSK','Position',[800,100,1100,900]);
subplot(6,6,1)
plot(real(qpsk_sig),imag(qpsk_sig),'o');
grid on
hold on
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
title('Original Constellation Diagram');
xlim([-2,2]);
ylim([-2,2]);
hold off
len = length(qpsk_sig);
R_Chann=sqrt(0.5)*(randn(1,len)+1i*randn(1,len));
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_QPSK_sig = qpsk_sig.*R_Chann + Noise; % Received signal
    Es_QPSK_sig = Rx_QPSK_sig./R_Chann;
    subplot(6,6,i+1)
    plot(real(Es_QPSK_sig(1:100)),imag(Es_QPSK_sig(1:100)),'*'); % Select some of the points
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    
    % Demodulation
    Bits4 = (real(Es_QPSK_sig*sqrt(2))<0);
    Bits3 = (imag(Es_QPSK_sig*sqrt(2))<0);
    Demod_qpsk_bits = zeros(1,2*length(Es_QPSK_sig)); % Demodulated bits
    Demod_qpsk_bits(1:2:end) = Bits3;
    Demod_qpsk_bits(2:2:end) = Bits4;
    Error_bits_QPSK = Bits_Sequence - Demod_qpsk_bits; %Calculate Bit Errors
    QPSK_R_BER_list(end+1) = sum(abs(Error_bits_QPSK))/N; % BER
end
 
%%
% R Channel to QAM16
SNR_dB=[0:1:30];
QAM16_R_BER_list=[];
figure('name','Rx_QAM16','Position',[200,100,1600,900]);
subplot(6,6,1)
plot(real(qam16_sig),imag(qam16_sig),'ro'); 
grid on
hold on
line([-3,3],[0,0],'Color','k');
line([0,0],[-3,3],'Color','k');
text(0.3562,0.3562,'1111','FontSize',12,'Interpreter','latex');
text(-0.3562,0.3562,'0111','FontSize',12,'Interpreter','latex');
text(-0.3562,-0.3562,'0011','FontSize',12,'Interpreter','latex');
text(0.3562,-0.3562,'1011','FontSize',12,'Interpreter','latex');
text(0.9987,0.9987,'1100','FontSize',12,'Interpreter','latex');
text(-0.9987,0.9987,'0100','FontSize',12,'Interpreter','latex');
text(-0.9987,-0.9987,'0000','FontSize',12,'Interpreter','latex');
text(0.9987,-0.9987,'1000','FontSize',12,'Interpreter','latex');
text(0.3562,0.9987,'1101','FontSize',12,'Interpreter','latex');
text(-0.3562,0.9987,'0101','FontSize',12,'Interpreter','latex');
text(-0.3562,-0.9987,'0001','FontSize',12,'Interpreter','latex');
text(0.3562,-0.9987,'1001','FontSize',12,'Interpreter','latex');
text(0.9987,0.3562,'1110','FontSize',12,'Interpreter','latex');
text(-0.9987,0.3562,'0110','FontSize',12,'Interpreter','latex');
text(-0.9987,-0.3562,'0010','FontSize',12,'Interpreter','latex');
text(0.9987,-0.3562,'1010','FontSize',12,'Interpreter','latex');
title('16QAM Constellation');
xlim([-2,2]);
ylim([-2,2]);
hold off
len = length(qam16_sig);
R_Chann=sqrt(0.5)*(randn(1,len)+1i*randn(1,len));
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_qam16_sig = qam16_sig.*R_Chann + Noise; % Received signal
    Es_qam16_sig = Rx_qam16_sig./R_Chann;
    subplot(6,6,i+1)
    plot(real(Es_qam16_sig(1:100)),imag(Es_qam16_sig(1:100)),'*'); % Select some of the points
    grid on
    hold on
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    % Demodulation
    ab = 1/sqrt(10);
    Bits6 = imag(Es_qam16_sig)>0;
    Bits7 = (imag(Es_qam16_sig)<2*ab) & (imag(Es_qam16_sig)>-2*ab);
    Bits5 = real(Es_qam16_sig)>0;
    Bits8 = (real(Es_qam16_sig)<2*ab) & (real(Es_qam16_sig)>-2*ab);
    % Combine into single stream
    comb = [Bits5; Bits6; Bits7; Bits8];
    Demod_qam16_bits = reshape(comb,1,4*length(comb));
    Error_bits_qam16 = Bits_Sequence - Demod_qam16_bits;	  % Calculate Bit Errors
    QAM16_R_BER_list(end+1) = sum(abs(Error_bits_qam16))/N;	% BER
end
 
%%
%64-QAM AWGN
% Split the bits into four streams
 
%Source
QAM64_Bits = reshape(Bits_Sequence,6,length(Bits_Sequence)/6);
Bits1 = QAM64_Bits(1,:);
Bits2 = QAM64_Bits(2,:);
Bits3 = QAM64_Bits(3,:);
Bits4 = QAM64_Bits(4,:);
Bits5 = QAM64_Bits(5,:);
Bits6 = QAM64_Bits(6,:);
 
QAM64_Bits = zeros(1,length(Bits_Sequence)/6);
for i=1:1:length(Bits_Sequence)/6
    QAM64_Bits(i)=32*Bits1(i)+16*Bits2(i)+8*Bits3(i)+4*Bits4(i)+2*Bits5(i)+Bits6(i);
end
 
% normalizing factor
normalizer = sqrt(1/42);	% (2*4+10*8+18*4+26*8+34*8+50*4+50*8+58*8+74*8+98*4)/64=42			
 
% bit mapping        
qam64_sig = normalizer*qammod(QAM64_Bits,64);
 
% Adds Gaussian Noise to QPSK signal
SNR_dB=[0:1:30];
QAM64_BER_list=[];
 
% Constellation of 64 QAM
figure('name','Rx_QAM64','Position',[200,100,1600,900]);
subplot(6,6,1)
plot(real(qam64_sig),imag(qam64_sig),'ro'); grid on;hold on;
line([-10,10],[0,0],'Color','k');
line([0,0],[-10,10],'Color','k');
title('64QAM Constellation');
xlim([-2,2]);
ylim([-2,2]);
hold off;
len = length(qam64_sig);
 
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_qam64_sig = qam64_sig + Noise; % Received signal
    subplot(6,6,i+1)
    plot(real(Rx_qam64_sig(1:100)),imag(Rx_qam64_sig(1:100)),'*'); % Select some of the points
    grid on;
    hold on;
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    
    % Demodulation
    de_qam64_sym=qamdemod(Rx_qam64_sig/normalizer,64);
    de_Bits1=zeros(1,length(de_qam64_sym));
    de_Bits2=zeros(1,length(de_qam64_sym));
    de_Bits3=zeros(1,length(de_qam64_sym));
    de_Bits4=zeros(1,length(de_qam64_sym));
    de_Bits5=zeros(1,length(de_qam64_sym));
    de_Bits6=zeros(1,length(de_qam64_sym));
    for j=1:1:length(de_qam64_sym)
        buf=de_qam64_sym(j);
        de_Bits1(j)=floor(buf/32);
        de_Bits2(j)=floor((buf-32*de_Bits1(j))/16);
        de_Bits3(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j))/8);
        de_Bits4(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j))/4);
        de_Bits5(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j)-4*de_Bits4(j))/2);
        de_Bits6(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j)-4*de_Bits4(j)-2*de_Bits5(j)));
    end
    
    % Combine into single stream
    comb = [de_Bits1; de_Bits2; de_Bits3; de_Bits4;de_Bits5;de_Bits6];
    Demod_qam64_bits = reshape(comb,1,6*length(comb));
    Error_bits_qam64 = Bits_Sequence - Demod_qam64_bits;	  % Calculate Bit Errors
    QAM64_BER_list(end+1) = sum(abs(Error_bits_qam64))/N;	% BER
end
 
%%
% QAM64 Passing through Rayleigh Channel
% normalizing factor
normalizer = sqrt(1/42);	% (2*4+10*8+18*4+26*8+34*8+50*4+50*8+58*8+74*8+98*4)/64=42			
 
% Adds Gaussian Noise to QPSK signal
SNR_dB=[0:1:30];
QAM64_R_BER_list=[];
 
% Constellation of 64 QAM
figure('name','Rx_QAM64','Position',[200,100,1600,900]);
subplot(6,6,1)
plot(real(qam64_sig),imag(qam64_sig),'ro'); grid on;hold on;
line([-10,10],[0,0],'Color','k');
line([0,0],[-10,10],'Color','k');
title('64QAM Constellation');
xlim([-2,2]);
ylim([-2,2]);
hold off;
len = length(qam64_sig);
R_Chann=sqrt(0.5)*(randn(1,len)+1i*randn(1,len));
 
for i=1:1:size(SNR_dB,2)
    N0 = 1/10^(SNR_dB(i)/10); % Noise variance
    Noise = sqrt(N0/2)*((randn(1,len)+1i*randn(1,len))); % AWGN Noise
    Rx_qam64_sig = R_Chann.*qam64_sig + Noise; % Received signal
    E_qam64_sig=Rx_qam64_sig./R_Chann;
    subplot(6,6,i+1)
    plot(real(E_qam64_sig(1:100)),imag(E_qam64_sig(1:100)),'*'); % Select some of the points
    grid on;
    hold on;
    line([-5,5],[0,0],'Color','k');
    line([0,0],[-5,5],'Color','k');
    title(['When The SNR is',num2str(SNR_dB(i))]);
    xlim([-4,4]);
    ylim([-4,4]);
    hold off
    
    % Demodulation
    
    de_qam64_sym=qamdemod(E_qam64_sig/normalizer,64);
    de_Bits1=zeros(1,length(de_qam64_sym));
    de_Bits2=zeros(1,length(de_qam64_sym));
    de_Bits3=zeros(1,length(de_qam64_sym));
    de_Bits4=zeros(1,length(de_qam64_sym));
    de_Bits5=zeros(1,length(de_qam64_sym));
    de_Bits6=zeros(1,length(de_qam64_sym));
    for j=1:1:length(de_qam64_sym)
        buf=de_qam64_sym(j);
        de_Bits1(j)=floor(buf/32);
        de_Bits2(j)=floor((buf-32*de_Bits1(j))/16);
        de_Bits3(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j))/8);
        de_Bits4(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j))/4);
        de_Bits5(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j)-4*de_Bits4(j))/2);
        de_Bits6(j)=floor((buf-32*de_Bits1(j)-16*de_Bits2(j)-8*de_Bits3(j)-4*de_Bits4(j)-2*de_Bits5(j)));
    end
    
    % Combine into single stream
    comb = [de_Bits1; de_Bits2; de_Bits3; de_Bits4;de_Bits5;de_Bits6];
    Demod_qam64_bits = reshape(comb,1,6*length(comb));
    Error_bits_qam64 = Bits_Sequence - Demod_qam64_bits;	  % Calculate Bit Errors
    QAM64_R_BER_list(end+1) = sum(abs(Error_bits_qam64))/N;	% BER
end
 
%%
figure('name','BER in different Channel','Position',[500,100,900,900]);
semilogy(SNR_dB,BPSK_BER_list,'r');text(4,0.002,'BPSK AWGN');
grid on;hold on;
semilogy(SNR_dB,QPSK_BER_list,'b');text(8,0.0005,'QPSK AWGN');
semilogy(SNR_dB,QAM16_BER_list,'g');text(14,0.0016,'QAM16 AWGN');
%semilogy(SNR_dB,BPSK_R_BER_list);text(18,0.0025,'BPSK Rayleigh');
%semilogy(SNR_dB,QPSK_R_BER_list);text(19,0.005,'QPSK Rayleigh');
%semilogy(SNR_dB,QAM16_R_BER_list);text(19,0.021,'QAM16 Rayleigh');
semilogy(SNR_dB,QAM64_BER_list);text(19,0.0085,'QAM64 AWGN');
%semilogy(SNR_dB,QAM64_R_BER_list);text(19,0.05,'QAM64 Rayleigh');
legend('BPSK-AWGN','QPSK-AWGN','QAM16-AWGN','BPSK-R','QPSK-R','QAM16-R','QAM64-AWGN','QAM64-R','Location','SouthWest');
title('BER in AWGN Channel');xlabel('SNR in dB');ylabel('BER');
 
%%
Eb_N0_bpsk=10*log10(10.^(SNR_dB/10));
Eb_N0_qpsk=10*log10(10.^(SNR_dB/10)/2);
Eb_N0_16qam=10*log10(10.^(SNR_dB/10)/4);
Eb_N0_64qam=10*log10(10.^(SNR_dB/10)/6);
figure('name','BER in different Channel','Position',[500,100,900,900]);
semilogy(Eb_N0_bpsk,BPSK_BER_list,'r');text(4,0.0008,'BPSK AWGN');
grid on;hold on;
semilogy(Eb_N0_qpsk,QPSK_BER_list,'b');text(3,0.002,'QPSK AWGN');
semilogy(Eb_N0_16qam,QAM16_BER_list,'g');text(10,0.0016,'QAM16 AWGN');
%semilogy(Eb_N0_bpsk,BPSK_R_BER_list);text(18,0.002,'BPSK Rayleigh');
%semilogy(Eb_N0_qpsk,QPSK_R_BER_list);text(15,0.005,'QPSK Rayleigh');
%semilogy(Eb_N0_16qam,QAM16_R_BER_list);text(12,0.02,'QAM16 Rayleigh');
semilogy(Eb_N0_64qam,QAM64_BER_list);text(10,0.008,'QAM64 AWGN');
%semilogy(Eb_N0_64qam,QAM64_R_BER_list);text(11,0.0536,'QAM64 Rayleigh');
legend('BPSK-AWGN','QPSK-AWGN','QAM16-AWGN','BPSK-R','QPSK-R','QAM16-R','QAM64-AWGN','QAM64-R','Location','SouthWest');
title('BER in AWGN Channel');xlabel('Eb/N0 in dB');ylabel('BER');