omegac=pi/2;
N=33;t=0:(2*pi/N):2*pi-(2*pi/N);
% N=N+1; % 现在是34了
Hd1=t; % 理想频响采样点
for i=1:length(t)
    if(t(i)<omegac||t(i)-2*pi>-omegac)
        Hd1(i)=1;
    else
        Hd1(i)=0;
    end
end

for i=1:length(t)-2
    if(Hd1(i)==0&&Hd1(i+1)==1&&Hd1(i+2)==1)
        Hd1(i)=0.33;Hd1(i+1)=0.66;
    else if(Hd1(i)==1&&Hd1(i+1)==0&&Hd1(i+2)==0)
            Hd1(i)=0.66;Hd1(i+1)=0.33;
        end
    end         
end

m=0:N-1;
Hd=Hd1.*exp(-1i*0.5*N*(2*pi*m./N));
H=zeros(1,10*N);
tt=0:2*pi/(10*N):2*pi-2*pi/(10*N);

for i=0:10*N-1
    if(mod(i,10)==0)
        H(i+1)=Hd(i/10+1);
    else % 内插公式
        H(i+1)=0;
        for k=0:N-1
            H(i+1)=H(i+1)+Hd(k+1)/(1-exp(1i*2*pi*k/N)*exp(-1i*2*pi*i/10/N));
        end
        H(i+1)=H(i+1)*(1-exp(-1i*N*2*pi*i/10/N))/N;
    end
end

plot(tt,abs(H),t,Hd1,'o');
title('实际滤波器的幅频响应和理想滤波器的采样点');

% subplot(121);
% plot(t,abs(H));
% title('实际滤波器幅频特性')
% grid on;
% subplot(122);
% plot(t,angle(H));
% title('实际滤波器相频特性')
% grid on;

% stem(1:length(h),h,'.');
% title('实际单位脉冲序列响应');