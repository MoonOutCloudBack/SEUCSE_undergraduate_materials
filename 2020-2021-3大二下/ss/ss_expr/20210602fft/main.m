f=[50,50,100,1000];
N=[32,64,32,32];
T=[0.005,0.005,0.0025,0.0012];

i=4;
samp=mySampling(f(i),N(i),T(i));
% stem((0:N(i)*2-1),samp);
% FFTresult=myFFT(samp);
% stem((0:N(i)*2-1).*(2/(N(i)*2)),abs(FFTresult)./max(abs(FFTresult)));
% plot((0:N(i)*2-1),conj(myFFT(conj(FFTresult))).*(1/(N(i)*2)));

xx=0:0.01:N(i)-1;
plot(0:N(i)-1,samp,'o',xx,interp1(0:N(i)-1,samp,xx,'spline'),'r');