omegap=0.2*pi;omegas=0.4*pi;
omegac=(omegap+omegas)/2;
N=100;t=0:(2*pi/N):2*pi;
Hd=t; % 理想频率响应特性
for i=1:length(t)
    if(t(i)<omegac||t(i)-2*pi>-omegac)
        Hd(i)=1;
    else
        Hd(i)=0;
    end
end
hd=conj(fft(conj(Hd)));hd=hd./max(hd); % 理想滤波器的单位脉冲响应
w=hamming(length(t))';
h=hd.*w;
H=fft(h);H=H./max(H);

subplot(121);
plot(t,abs(H));
title('实际滤波器幅频特性')
grid on;
subplot(122);
plot(t,angle(H));
title('实际滤波器相频特性')
grid on;

% stem(1:length(h),h,'.');
% title('实际单位脉冲序列响应');




