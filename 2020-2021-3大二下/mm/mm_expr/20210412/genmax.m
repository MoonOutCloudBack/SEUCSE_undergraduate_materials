function [a]=genmax(n)
% 输入为阶数n，输出为随机构造的正互反矩阵; generate matrix.
b=[[1:9],1/2,1/3,1/4,1/5,1/6,1/7,1/8,1/9];
a=ones(n,n);
for i=2:n
    for j=1:i-1
        a(i,j)=b(mod(floor(rand(1)*100000),17)+1);
        a(j,i)=1/a(i,j);
    end
end
end