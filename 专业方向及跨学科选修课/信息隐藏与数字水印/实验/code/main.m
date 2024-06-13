%%
clear;
clc;
close all;
      
%%
watermark_img=imread('seu_60_60.png');
%watermark_img= imresize(watermark_img, [60 60]);
%imwrite(watermark_img, 'seu_60_60.png');
watermark_img=rgb2gray(watermark_img);
watermark_img=im2bw(watermark_img);    %使水印图像变为二值图
%figure(1);      %打开窗口
%subplot(2,3,1);    %该窗口内的图像可以有两行三列
%imshow(watermark_img),title('水印图像');   %显示水印图像
[rm,cm]=size(watermark_img);   %计算水印图像的长宽

%%
cover_image=imread('lena_640_480.bmp');
cover_image=im2gray(cover_image);
%subplot(2,3,2),imshow(cover_image,[]),title('载体图像'); %[]表示显示时灰度范围为image上的灰度最小值到最大值

%%
before=blkproc(cover_image,[8 8],'dct2');   %将载体图像的灰度层分为8×8的小块，每一块内做二维DCT变换，结果记入矩阵before

%%
I=watermark_img;
alpha=30;     %尺度因子,控制水印添加的强度,决定了频域系数被修改的幅度
k1=randn(1,8);  %产生两个不同的随机序列
k2=randn(1,8);
after=before;   %初始化载入水印的结果矩阵
for i=1:rm          %在中频段嵌入水印
    for j=1:cm
        x=(i-1)*8;
        y=(j-1)*8;
        if watermark_img(i,j)==1
            k=k1;
        else
            k=k2;
        end
        after(x+1,y+8)=before(x+1,y+8)+alpha*k(1);
        after(x+2,y+7)=before(x+2,y+7)+alpha*k(2);
        after(x+3,y+6)=before(x+3,y+6)+alpha*k(3);
        after(x+4,y+5)=before(x+4,y+5)+alpha*k(4);
        after(x+5,y+4)=before(x+5,y+4)+alpha*k(5);
        after(x+6,y+3)=before(x+6,y+3)+alpha*k(6);
        after(x+7,y+2)=before(x+7,y+2)+alpha*k(7);
        after(x+8,y+1)=before(x+8,y+1)+alpha*k(8);
    end
end
result=blkproc(after,[8 8],'idct2');    %将经处理的图像分为8×8的小块，每一块内做二维DCT逆变换
result = uint8(result);
%imwrite(result,'markresule.bmp','bmp');      %存储添加水印后的图像
%subplot(2,3,3),imshow(result,[]),title('嵌入水印的图像');    %显示添加水印后的图像
 
%%
disp('请选择对图像的攻击方式：');
disp('1.添加白噪声');
disp('2.高斯低通滤波处理');
disp('3.对图像进行部分剪切');
disp('4.将图像旋转十度');
disp('5.将图像压缩处理');
disp('6.添加椒盐噪声');
disp('7.不处理图像，直接显示提取水印');
disp('输入其它数字则直接显示提取水印');
choice=input('请输入选择：');
figure(1);
switch choice        %读入输入的选择  withmark为等待提取水印的图像
    case 1
        result_1=result;
        withmark=imnoise(result_1,'gaussian',0.1); %加入高斯躁声
        imwrite(withmark, 'gaussian_0.1_lena.png');
        %subplot(2,3,4);
        %imshow(withmark,[]);
        %title('加入高斯白噪声后的图像');     %显示加了高斯噪声的图像
    case 2
         result_2=result;
         H=fspecial('gaussian',[4,4],1); 
         result_2=imfilter(result_2,H); 
         %subplot(2,3,4); 
         %imshow(result_2,[]); 
         %title('高斯低通滤波后图像'); 
         withmark=result_2;
         imwrite(withmark, 'filter_1_lena.png');
    case 3
        result_3=result;
        result_3(1:64,1:400)=512;   %使图像上方被剪裁
        %subplot(2,3,4);
        %imshow(result_3);
        %title('上方剪切后图像');
        withmark=result_3;
        imwrite(withmark, 'cut_lena.png');
    case 4
        result_4=imrotate(result,180,'bilinear','crop');   %最邻近线性插值算法旋转10度
        subplot(2,3,4);
        %imshow(result_4);
        %title('旋转10度后图像');
        withmark=result_4;
        imwrite(withmark, 'rotate_10_lena.png');
    case 5
        result_5 = result;
        result_5 = im2double(result_5);
        imwrite(result_5,'test.jpg','quality',50);
        J = imread('test.jpg');
        withmark=J;
        imwrite(withmark, 'compress_50_lena.png');
    case 6
        result_6=result;
        withmark=imnoise(result_6,'salt & pepper',0.02); %加入椒盐躁声
        %subplot(2,3,4);
        %imshow(withmark,[]);
        %title('加入椒盐噪声后的图像');     %显示加了椒盐噪声的图像
    case 7
        %subplot(2,3,4);
        %imshow(result,[]);
        %title('未受攻击的水印图像');
        withmark=result;
    otherwise
        disp('输入数字选择无效，图像未受攻击，直接提取水印');
        %subplot(2,3,4);
        %imshow(result,[]);
        %title('未受攻击的水印图像');
        withmark=result;
end



%%
after_2=blkproc(withmark,[8,8],'dct2');   %此步开始提取水印，将灰度层分块进行DCT变换
p=zeros(1,8);        %初始化提取数值用的矩阵
mark_2 = zeros(rm,cm);
for i=1:rm
    for j=1:cm
        x=(i-1)*8;
        y=(j-1)*8;
        p(1)=after_2(x+1,y+8);         %将之前改变过数值的点的数值提取出来
        p(2)=after_2(x+2,y+7);
        p(3)=after_2(x+3,y+6);
        p(4)=after_2(x+4,y+5);
        p(5)=after_2(x+5,y+4);
        p(6)=after_2(x+6,y+3);
        p(7)=after_2(x+7,y+2);
        p(8)=after_2(x+8,y+1);
        if corr2(p,k1)>corr2(p,k2)  %corr2计算两个矩阵的相似度，越接近1相似度越大
            mark_2(i,j)=1;              %比较提取出来的数值与随机频率k1和k2的相似度，还原水印图样
        else
            mark_2(i,j)=0;
        end
    end
end
%subplot(2,3,5);
imwrite(mark_2, 'compress_50_seu.bmp');
mark_2 = uint8(mark_2);

NC=correlation(mark_2,watermark_img);  
disp('原水印图像与提取水印图像互相关系数:')
disp(NC);

