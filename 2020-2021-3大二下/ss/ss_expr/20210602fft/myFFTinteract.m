function X=myFFTinteract(x)
x0=x;N=length(x);
while(bitand(N,1)==0&&N>1)
    N=N/2;
end
if N~=1
    zerofill=input("输入序列长度不是2的幂，请输入是(1)否(0)补零：");
    if zerofill==0
        fillvalue=input("请输入补全的数值：");
        N=length(x);
        x0=[x0;ones(2^ceil(log(N)/log(2))-N,1).*fillvalue];
    end
end
X=myFFT(x0);
end

% test=unifrnd(-1,1,16,1);
% myFFTinteract(test)