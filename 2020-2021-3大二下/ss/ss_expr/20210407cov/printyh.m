function printyh(n)
% 打印对应阶数的杨辉三角形, 输入为阶数，无输出
yh=1;disp(yh);
for j=2:n
    old=yh;
    yh=ones(1,length(old)+1);
    for k=2:length(old)
        yh(k)=old(k-1)+old(k);
    end
    disp(yh);
end