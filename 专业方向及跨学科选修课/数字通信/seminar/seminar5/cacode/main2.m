%% 第 1 个 C/A code
ca1 = C_A_Code(1);
x = [1:1023];
plot(x, ca1)
axis([0, 1024, -0.1, 1.1]);
title('第 1 个 C/A code');
set(gca, 'YTick', [0, 1]);
xlabel('相位');
ylabel('比特');
%% 第 2 个 C/A code
ca2 = C_A_Code(2);
x = [1:1023];
plot(x, ca2)
axis([0, 1024, -0.1, 1.1]);
title('第 2 个 C/A code');
set(gca, 'YTick', [0, 1]);
xlabel('相位');
ylabel('比特');
%% 第 1 个 C/A code 自相关
ca1 = C_A_Code(1);
ca1 = (ca1 * 2) - 1;
x = [1:2045];
y = xcorr(ca1);
plot(x, y)
axis([0, 2046, -100, 1200]);
xlabel('相位');
ylabel('相关值');
title('第 1 个 C/A code 自相关')
%% 第 1 和第 2 个 C/A code 互相关
ca1 = C_A_Code(1);
ca1 = (ca1 * 2) - 1;
ca2 = C_A_Code(2);
ca2 = (ca2 * 2) - 1;
x = [1:2045];
y = xcorr(ca1, ca2);
plot(x, y)
axis([0, 2046, -100, 1200]);
xlabel('相位');
ylabel('相关值');
title('第 1 和第 2 个 C/A code 互相关')
%% 第 1 个 C/A code 有相移的互相关
ca1 = C_A_Code(1);
ca1 = (ca1 * 2) - 1;
ca2 = [ca1(500:end), ca1(1:499)];
x = [1:2045];
y = xcorr(ca1, ca2);
plot(x, y)
axis([0, 2046, -100, 1200]);
xlabel('相位');
ylabel('自相关值');
title('第 1 个 C/A code 有相移的互相关')
%% 最大相关幅值与相位差的关系
ca1 = C_A_Code(1);
ca1 = (ca1 * 2) - 1;
y = zeros(1, 1023);
for i = [2:1023]
    ca2 = [ca1(i:end), ca1(1:i-1)];
    [y(i), p] = max(xcorr(ca1, ca2));
end
[y(1), p]= max(xcorr(g1));
y = [y(512:end), y(1:511)];
x = [-1:(1 / 511):1];
plot(x, y)
axis([-1, 1, 512, 1050]);
xlabel('相位差/码片');
ylabel('最大相关幅值');
title('最大相关幅值与相位差的关系')
%% 码环鉴别器对比
ca1 = C_A_Code(1);
ca1 = (ca1 * 2) - 1;
y = zeros(1, 1023);
for i = [2:1023]
    ca2 = [ca1(i:end), ca1(1:i-1)];
    [y(i), p] = max(xcorr(ca1, ca2));
end
[y(1), p]= max(xcorr(ca1));
y = [y(512:end), y(1:511)];

delta1 = zeros(1, 1023);
delta2 = zeros(1, 1023);
delta3 = zeros(1, 1023);
E = 0;
L = 0;
for i = [1:1023]
    if i > 256
        E = y(i - 256) - 512;
    end
    if i < 512 + 256
        L = y(i + 256) - 512;
    end
    P = y(i) - 512;
    delta1(i) = (E - L) / (E + L) / 2;
    delta2(i) = (E * E - L * L) / (E * E + L * L) / 2;
    delta3(i) = (E - L) / P / 4;
end
x = [-1:(1 / 511):1];
plot(x, delta1)
hold on;
plot(x, delta2)
hold on;
plot(x, delta3)
legend('幅值法','功率法','点积功率法');
axis([-1, 1, -1, 1]);
xlabel('相位差/码片');
ylabel('\delta_{cp}/码片');
title('码环鉴别器对比')
