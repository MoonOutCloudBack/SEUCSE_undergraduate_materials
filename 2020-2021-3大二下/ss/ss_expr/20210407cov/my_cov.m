function [y]=my_cov(x,h)
% 返回两个数组的卷积和，x和h是数组，函数体内没有输出
y=zeros(1,length(x)+length(h)-1);
for n=0:length(x)+length(h)-2
    for k=max(0,n+1-length(h)):min(length(x)-1,n)
        y(n+1)=y(n+1)+x(k+1)*h(n-k+1);
    end
end

% y=my_cov(x,h)
% plot([0:length(y)-1],y)