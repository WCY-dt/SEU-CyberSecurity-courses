number=input('Please input a number between 1 and 32:');
G1=ones(1,10);
G2=ones(1,10);
m=ones(1,3);
Code2= zeros(1,1);
G2Table=[2,3,4,5,1,2,1,2,3,2,3,5,6,7,8,9,1,2,3,4,5,6,1,4,5,6,7,8,1,2,3,4;
6,7,8,9,9,10,8,9,10,3,4,6,7,8,9,10,4,5,6,7,8,9,3,6,7,8,9,10,6,7,8,9];

for i=1:1023
R(1)=mod(G2(G2Table(1,number))+G2(G2Table(2, number)),2);
rd(i)=mod(R(1)+G1(10),2);
newBit1=[mod(G1(3)+G1(10),2)];
G1=[newBit1 G1(1:9)];
newBit2=[mod(G2(2)+G2(3)+G2(6)+G2(8)+G2(9)+G2(10),2)];
G2=[newBit2 G2(1:9)];
end
cacode=repmat(rd,1,2);

for i=1:2046
if cacode(i)==0
    cacode(i)=1;
else
    cacode(i)=-1;
end
end
k=length(cacode);
xk=fft(cacode,k);
rm=real(ifft(conj(xk).*xk))/2046;
rm=ifftshift(rm);
axis([0,2000,-0.2,1]);
plot(rm)
set(gca,'xtick',[0:80:k-1]);
xlabel('Code Phase')
ylabel('Normalized Correlation Fuction')
title('Autocorrelation of C/A code')

