result=zeros(30,1);
for n=3:30
    s=0;c=0;  % sum和count
    k=100000;kold=0;
    m=1000;
    while abs(k-kold)>0.0005
        kold=k;
        for i=1:m
            [x,y]=eig(genmax(n));
            s=s+max(diag(y));
        end
        c=c+m;
        k=s/c;
    end
    ri=(k-n)/(n-1);
    result(n)=ri;
end

disp(result);