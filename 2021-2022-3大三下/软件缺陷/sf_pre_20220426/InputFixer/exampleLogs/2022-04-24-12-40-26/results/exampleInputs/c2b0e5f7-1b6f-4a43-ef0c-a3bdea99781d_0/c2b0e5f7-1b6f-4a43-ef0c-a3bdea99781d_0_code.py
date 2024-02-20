n, m = (int(i) for i in input().split())
#a=[[input() for j in range(m)] for i in range(n-1)]
a=[]

for i in range(n):
    if i != n-1:
        b=[]
        for j in range(m):
            b.append(int(input()))
        a.append(b)
        print(a)
    else:
        a.append('end')
print(a)
for i in range(n):
    for j in range(m):
        print(a[i][j], end=' ')
    print()
c=[[0 for j in range(m)] for i in range(n-1)]

if len(a)==2:
    c[0][0]=a[0][0]*4

for i in range(n-1):
    for j in range(m):
        for r in range(-1, 2):
            for d in range(-1,2):
                if r-d!=0 and abs(r-d)!=2:
                    if 0<=i+r<n-2 and 0<=j+d<m-1:
                        c[i][j]+=a[i+r][j+d]
                        
                    elif i+r<0 and 0<=j+d<m-1:
                        c[i][j]+=a[n-2][j+d]
                    elif i+r>n-2 and 0<=j+d<m-1:
                        c[i][j]+=a[0][j+d]  
                    elif j+d<0 and 0<=i+r<n-2:
                        c[i][j]+=a[i+r][m-1]
                    elif j+d>m-1 and 0<=i+r<n-2:
                        c[i][j]+=a[i+r][0] 
                    print(c, i, j, i+r, j+d)
                    

for i in range(n-1):
    for j in range(m):
        print(c[i][j], end=' ')
    print()