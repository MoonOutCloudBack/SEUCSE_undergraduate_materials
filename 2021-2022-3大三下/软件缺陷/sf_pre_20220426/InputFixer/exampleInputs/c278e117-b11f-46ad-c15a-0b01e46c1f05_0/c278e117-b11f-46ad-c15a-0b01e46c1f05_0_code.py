n=int(input())
num=[int(input()) for i in range(n)]
res=[]
for s in range(1,n):
    for u in range(1,n):
        res.append(num[s]-num[u])