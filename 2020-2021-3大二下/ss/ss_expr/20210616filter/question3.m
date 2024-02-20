% while(data(i)==0) % 预处理
%     i=i+1;
% end
% data=data(i:length(data));data=data./max(data);
L=65536;wave=data(1:L);
% subplot(211);
% plot(1:L,wave);
% title('时域前65536点的波形');
% freq=fft(wave);freq=freq./max(freq);
% subplot(212);
% plot((1:L)./(L/2/pi),abs(freq));
% title('前65536点的频谱（幅频特性）');


N=65536;wave=data(1:N);
freq=fft(wave);freq=freq./max(freq);
tp=8.08*(N/391168);Deltaf=1/tp;

% % 低通滤波
% fp=1000;fs=1200;
% omegap=fp/Deltaf*2*pi/L;omegas=fs/Deltaf*2*pi/L;
% omegac=(omegap+omegas)/2;
% t=0:(2*pi/N):2*pi-(2*pi/N);
% Hd=t; % 理想频率响应特性
% for i=1:length(t)
%     if(t(i)<omegac||t(i)-2*pi>-omegac)
%         Hd(i)=1;
%     else
%         Hd(i)=0;
%     end
% end
% hd=conj(fft(conj(Hd)));hd=hd./max(hd); % 理想滤波器的单位脉冲响应
% w=hamming(length(t))';
% h=hd.*w;
% H=fft(h)';H=H./max(H);
% low_fil_freq=H.*freq;
% low_fil_freq=low_fil_freq./max(low_fil_freq);
% low_fil_wave=conj(fft(conj(low_fil_freq))');
% low_fil_wave=low_fil_wave./max(low_fil_wave);
% % subplot(211);
% % plot(1:N,low_fil_wave);
% % title('低通滤波后时域前65536点的波形');
% % subplot(212);
% % plot((0:N-1).*Deltaf,abs(low_fil_freq));
% % title('低通滤波后前65536点的频谱（幅频特性）');

% % 一个魔改
% for i=0.75*N:N
%    low_fil_freq(i)=0;
% end
% low_fil_freq=low_fil_freq./max(low_fil_freq);
% low_fil_wave=conj(fft(conj(low_fil_freq))');
% low_fil_wave=low_fil_wave./max(low_fil_wave);
% subplot(211);
% plot(1:N,low_fil_wave);
% title('低通滤波后时域前65536点的波形');
% subplot(212);
% plot((0:N-1).*Deltaf,abs(low_fil_freq));
% title('低通滤波后前65536点的频谱（幅频特性）');

% 高通滤波
fp=4800;fs=5000;
omegap=fp/Deltaf*2*pi/L;omegas=fs/Deltaf*2*pi/L;
omegac=(omegap+omegas)/2;
t=0:(2*pi/N):2*pi-(2*pi/N);
Hd=t; % 理想频率响应特性
for i=1:length(t)
    if(t(i)<omegac||t(i)-2*pi>-omegac)
        Hd(i)=0;
    else
        Hd(i)=1;
    end
end
hd=conj(fft(conj(Hd)));hd=hd./max(hd); % 理想滤波器的单位脉冲响应
w=hamming(length(t))';
h=hd.*w;
H=fft(h)';H=H./max(H);
high_fil_freq=H.*freq;
high_fil_freq=high_fil_freq./max(high_fil_freq);
high_fil_wave=conj(fft(conj(high_fil_freq))');
high_fil_wave=high_fil_wave./max(high_fil_wave);
% subplot(211);
% plot(1:N,high_fil_wave);
% title('高通滤波后时域前65536点的波形');
% subplot(212);
% plot((0:N-1).*Deltaf,abs(high_fil_freq));
% title('高通滤波后前65536点的频谱（幅频特性）');

% 一个魔改
for i=1:0.5*N
   high_fil_freq(i)=0;
end
high_fil_freq=high_fil_freq./max(high_fil_freq);
high_fil_wave=conj(fft(conj(high_fil_freq))');
high_fil_wave=high_fil_wave./max(high_fil_wave);
subplot(211);
plot(1:N,high_fil_wave);
title('高通滤波后时域前65536点的波形');
subplot(212);
plot((0:N-1).*Deltaf,abs(high_fil_freq));
title('高通滤波后前65536点的频谱（幅频特性）');


