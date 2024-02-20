function printyh1(n)
% 打印对应阶数的杨辉三角形, 输入为阶数，无输出
yh=1; disp(yh);
for k=2:n
    yh=[yh,0]+[0,yh];
    disp(yh);
end

% 1 5 10 10 5  1
%   1 5  10 10 5 1
% 1 6 15 20 15 6 1