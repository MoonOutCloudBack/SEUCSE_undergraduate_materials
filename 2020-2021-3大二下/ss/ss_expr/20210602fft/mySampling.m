function [x]=mySampling(f,N,T)
x=zeros(N,1);
for n=0:N-1
    x(n+1)=sin(2*pi*f*n*T);
end
end