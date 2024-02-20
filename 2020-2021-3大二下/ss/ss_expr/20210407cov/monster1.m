skill=[5,4,3,2,1];
acti=[1,1,1];
hurt=my_cov(skill,acti);
total=zeros(1,length(hurt));
total(1)=hurt(1);
for i=2:length(hurt)
    total(i)=total(i-1)+hurt(i);
end
disp(total);
stem(total,'fill');