firstsen =input().strip().upper()
word =input().strip().upper()
i=0
c=0
for i in range(len(firstsen)):
    firstword=firstsen.find(" ")
    f1=0
    word2=firstsen[f1:firstword]
    if word2==word:
        print('Found')
        c=c+1
        break
    f1=firstword
    firstsen=fristsen[firstword+1:len(firstsen)]
if c==0:
    print('Not Found')


