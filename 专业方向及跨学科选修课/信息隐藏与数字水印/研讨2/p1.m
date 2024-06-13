clear;
clc;
hidefid = fopen('隐藏.txt', 'r');
[msg, count] = fread(hidefid, inf, 'ubit1');
fid = fopen('待隐藏.mid', 'r');
[wa, length] = fread(fid, inf, 'uint8');
fclose(hidefid);
fclose(fid);
modcount = 1;
p = 1;

i = 1;
while i <= length
    if bitand(wa(i, 1), 240) == 144 && p <= count %声音开启
        wa(i + 2, 1) = bitset(wa(i + 2, 1), 1, msg(p, 1));
        p = p + 1;
        i = i + 3;
        modcount = modcount + 1;
    elseif bitand(wa(i, 1), 240) == 192 && p <= count %乐器编号
        wa(i + 1, 1) = bitset(wa(i + 1, 1), 1, msg(p, 1));
        p = p + 1;
        i = i + 2;
        modcount = modcount + 1;
    elseif bitand(wa(i, 1), 240) == 208 && p <= count - 3 %通道触动压力
        wa(i + 1, 1) = bitset(wa(i + 1, 1), 1, msg(p, 1));
        wa(i + 1, 1) = bitset(wa(i + 1, 1), 2, msg(p + 1, 1));
        wa(i + 1, 1) = bitset(wa(i + 1, 1), 3, msg(p + 2, 1));
        wa(i + 1, 1) = bitset(wa(i + 1, 1), 4, msg(p + 3, 1));
        p = p + 4;
        i = i + 2;
        modcount = modcount + 1;
    else
        i = i + 1;
    end
end

fid = fopen('隐藏.mid', 'a');
fwrite(fid, wa);
fclose(fid);