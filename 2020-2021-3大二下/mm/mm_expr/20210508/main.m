a=1;b=0.1;c=0.5;d=0.02;
xinit=25;yinit=2;
sigma=0.5;

[t,x0,y0]=gendata(a,b,c,d,xinit,yinit);
% plot(t,x0,t,y0);
% title('数值仿真结果');legend('食饵','捕食者');
% xlabel('time');ylabel('number');

[t,x,y]=addnoise(sigma,t,x0,y0);
% plot(t,x,t,y);
% title('添加随机干扰后的数据');legend('食饵','捕食者');
% xlabel('time');ylabel('number');

[a1,b1,c1,d1,T,xinit1,yinit1]=calcpara(t,x,y);
disp([a1,b1,c1,d1,T,xinit1,yinit1]);