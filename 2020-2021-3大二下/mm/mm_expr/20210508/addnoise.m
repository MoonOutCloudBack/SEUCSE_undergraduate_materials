function [t,x,y]=addnoise(sigma,t,x0,y0)
% 输入为比例系数kx kx和原始数据t x0 y0，输出扰动后的数据[t,x,y]
x=x0;y=y0;quant=50;
for i=1:50:length(t)
    dx=normrnd(0,sigma,1);
    dy=normrnd(0,sigma,1);
    for j=i:min([i+quant,length(t)])
        x(j)=x(j)+dx;
        y(j)=y(j)+dy;
    end
end
end