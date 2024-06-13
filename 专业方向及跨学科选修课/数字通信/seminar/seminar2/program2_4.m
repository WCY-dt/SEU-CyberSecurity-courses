M=8;         %8QAM
k=log2(M);   %每个码元对应的bit数
n=1.2e6;     %比特序列长度
samp=1;      %过采样率
coderate=1;  %编码码率
x=randi([0 1],1,n);                %产生1行n列即n个取值为0或1的随机数组
xreshape=reshape(x, n/k, k);   %将二进制比特流数组转换为n/k行k列的符号数组xsym=bi2de (xreshape,'left-msb’) ;
xsym=bi2de(xreshape,'left-msb');

h_mod1=modem.genqammod('Constellation',[-(2+3^0.5)^0.5+1j*(2+3^0.5)^0.5,(2+3^0.5)^0.5+1j*(2+3^0.5)^0.5,1j*2^0.5,2^0.5,-(2+3^0.5)^0.5-1j*(2+3^0.5)^0.5,(2+3^0.5)^0.5-1j*(2+3^0.5)^0.5,-2^0.5,-1j*2^0.5,], 'InputType', 'integer');
h_demod1 = modem.genqamdemod('Constellation',[-(2+3^0.5)^0.5+1j*(2+3^0.5)^0.5,(2+3^0.5)^0.5+1j*(2+3^0.5)^0.5,1j*2^0.5,2^0.5,-(2+3^0.5)^0.5-1j*(2+3^0.5)^0.5,(2+3^0.5)^0.5-1j*(2+3^0.5)^0.5,-2^0.5,-1j*2^0.5,],'OutputType', 'integer');
h_mod2 = modem.genqammod('Constellation', [ -3+1j*1,-3-1j*1,-1+1j*1,-1-1j*1,1+1j*1,1-1j*1,3+1j*1,3-1j*1,],'InputType','integer');
h_demod2 = modem.genqamdemod('Constellation',[ -3+1j*1,-3-1j*1,-1+1j*1,-1-1j*1,1+1j*1, 1-1j*1,3+1j*1,3-1j*1,],'OutputType','integer');
h_mod3 = modem.genqammod('Constellation', [ -1+1j*3,-1+1j*1,-1-1j*1,-1-1j*3,1+1j*3,1+1j*1,1-1j*1,1-1j*3,],'InputType','integer');
h_demod3 = modem.genqamdemod( 'Constellation', [ -1+1j*3,-1+1j*1,-1-1j*1,-1-1j*3,1+1j*3,1+1j*1,1-1j*1,1-1j*3,],'OutputType', 'integer');
h_mod4 = modem.genqammod('Constellation',4*exp(1j*2*pi*[0:M-1]/M),'InputType','integer');
h_demod4 = modem.genqamdemod('Constellation' ,4*exp(1j*2*pi*[0:M-1]/M) ,'OutputType','integer');

y1=modulate(h_mod1, xsym);
y2=modulate(h_mod2, xsym);
y3=modulate(h_mod3, xsym);
y4=modulate(h_mod4, xsym);
snr = 7:1:20;
num=length(snr);
num_of_errors=zeros(4,num);
bit_error_rate=zeros(4,num) ;

for m=1:num
yn1=awgn(y1, snr(m), 'measured');
yn2=awgn(y2, snr(m), 'measured');
yn3=awgn(y3, snr(m), 'measured');
yn4=awgn(y4, snr(m), 'measured');

zsym1=demodulate(h_demod1, yn1);
zsym2=demodulate(h_demod2, yn2);
zsym3=demodulate(h_demod3, yn3);
zsym4=demodulate(h_demod4, yn4);

z1 = reshape(de2bi(zsym1,'left-msb'),1,n);
z2 = reshape(de2bi(zsym2,'left-msb'),1,n);
z3 = reshape(de2bi(zsym3,'left-msb'),1,n);
z4 = reshape(de2bi(zsym4,'left-msb'),1,n);

[num_of_errors(1,m),bit_error_rate(1,m)]=biterr(x,z1);
[num_of_errors(2,m),bit_error_rate(2,m)]=biterr(x,z2);
[num_of_errors(3,m),bit_error_rate(3,m)]=biterr(x,z3);
[num_of_errors(4,m),bit_error_rate(4,m)]=biterr(x,z4);
end

plot(snr, log10(bit_error_rate(1,1:num)),'-+k');
xlabel('SNR/dB'),ylabel('BER');
title( 'BER曲线'); 
axis([8 16 -5 -1]);
grid on
hold on
plot(snr,log10(bit_error_rate(2, 1:num)),'-ob');
hold on
plot(snr,log10(bit_error_rate(3, 1:num)),'-xr');
hold on
plot(snr,log10(bit_error_rate(4, 1:num)) ,'-*m');
legend('最优','横矩形','竖矩形','8PSK');

