%%
clear;
clc;
close all;

%%
watermark_img_original = imread('seu_60_60.png');
watermark_img_original = rgb2gray(watermark_img_original);
watermark_img_original = im2bw(watermark_img_original);
[row, column] = size(watermark_img_original);

%%
carrier_img_original = imread('lena_640_480.bmp');
carrier_img_original = im2gray(carrier_img_original);

%%
before = blkproc(carrier_img_original, [8 8], 'dct2');

%%
alpha = 30;
k1 = randn(1, 8);
k2 = randn(1, 8);
after = before;

for i = 1:row

    for j = 1:column
        x = (i - 1) * 8;
        y = (j - 1) * 8;

        if watermark_img_original(i, j) == 1
            k = k1;
        else
            k = k2;
        end

        after(x + 1, y + 8) = before(x + 1, y + 8) + alpha * k(1);
        after(x + 2, y + 7) = before(x + 2, y + 7) + alpha * k(2);
        after(x + 3, y + 6) = before(x + 3, y + 6) + alpha * k(3);
        after(x + 4, y + 5) = before(x + 4, y + 5) + alpha * k(4);
        after(x + 5, y + 4) = before(x + 5, y + 4) + alpha * k(5);
        after(x + 6, y + 3) = before(x + 6, y + 3) + alpha * k(6);
        after(x + 7, y + 2) = before(x + 7, y + 2) + alpha * k(7);
        after(x + 8, y + 1) = before(x + 8, y + 1) + alpha * k(8);
    end

end

result = blkproc(after, [8 8], 'idct2');
result = uint8(result);

%%
carrier_img_embeded = result;

%%
before = blkproc(carrier_img_embeded, [8, 8], 'dct2');
after = zeros(1, 8);
watermark_img_extract = zeros(row, column);

for i = 1:row

    for j = 1:column
        x = (i - 1) * 8;
        y = (j - 1) * 8;
        after(1) = before(x + 1, y + 8);
        after(2) = before(x + 2, y + 7);
        after(3) = before(x + 3, y + 6);
        after(4) = before(x + 4, y + 5);
        after(5) = before(x + 5, y + 4);
        after(6) = before(x + 6, y + 3);
        after(7) = before(x + 7, y + 2);
        after(8) = before(x + 8, y + 1);

        if corr2(after, k1) > corr2(after, k2)
            watermark_img_extract(i, j) = 1;
        else
            watermark_img_extract(i, j) = 0;
        end

    end

end

imwrite(watermark_img_extract, 'seu_extract.png');
watermark_img_extract = uint8(watermark_img_extract);

NC = correlation(watermark_img_extract, watermark_img_original);
disp(NC);
