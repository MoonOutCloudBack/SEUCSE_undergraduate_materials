function [X]=myFFT(x)
% 输入为N点有限长序列x（列向量），输出为其离散傅里叶变换X（列向量），要求N为2的幂
% 使用按时间抽取的FFT算法
N=length(x);X=zeros(N,1);
if N==2 % 递归边界条件
    X(1)=x(1)+x(2);X(2)=x(1)-x(2);return;  
end

x1=zeros(N/2,1);x2=x1;
for r=0:N/2-1
    x1(r+1)=x(2*r+1); % 偶数位
    x2(r+1)=x(2*r+1+1); % 奇数位
end
X1=myFFT(x1);X2=myFFT(x2); % 递归调用！

W=exp(-1i*2*pi/N);
for k=0:N/2-1
    X(k+1)=X1(k+1)+W^k*X2(k+1); % 计算X
    X(k+N/2+1)=X1(k+1)-W^k*X2(k+1);
end
end