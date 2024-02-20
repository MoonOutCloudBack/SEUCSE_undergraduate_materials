function [a]=my_cov2d(f,g)
% 输出为两个矩阵的卷积结果，输入为两个矩阵，函数体内没有输出
[rf,cf]=size(f);[rg,cg]=size(g);
a=zeros(rf+rg-1,cf+cg-1);
for x=0:rf+rg-2
    for y=0:cf+cg-2
        for k1=max(0,x+1-rg):min(rf-1,x)
            for k2=max(0,y+1-cg):min(cf-1,y)
                a(x+1,y+1)=a(x+1,y+1)+f(k1+1,k2+1)*g(x-k1+1,y-k2+1);
            end
        end
    end
end
