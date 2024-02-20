skill=[5,4,3,2,1];
hurt=[skill,0,0]+[0,skill,0]+[0,0,skill];
total=zeros(1,length(hurt));
total(1)=hurt(1);
for i=2:length(hurt)
    total(i)=total(i-1)+hurt(i);
end
disp(total);
stem(total,'fill');