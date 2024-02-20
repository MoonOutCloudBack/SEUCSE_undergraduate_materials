function [a,b,c,d,T,xinit,yinit]=calcpara(t,x,y)
% get a b c d
lnx=log(x);lny=log(y);
len=length(t);
Y=y';X=[lny',lnx',-x',ones(len,1)];
[para,~,~,~,~]=regress(Y,X); % a,c,d,C
b=0.1;a=abs(b*para(1));
c=abs(b*para(2));d=abs(b*para(3));

% get T
T=0;last=0;count=0;
for i=11:10:len-10
    if x(i)>98&&x(i)>x(i-10)&&x(i)>x(i+10)
        if last==0
            last=t(i);continue
        end
        count=count+1;
        T=T+t(i)-last;last=t(i);
    end
end
T=T/count;

% get xinit,yinit
count=0;xinit=0;yinit=0;quant=T*100;
for i=1:quant:len
    xinit=xinit+x(i);yinit=yinit+y(i);count=count+1;
end
xinit=xinit/count;yinit=yinit/count;

end