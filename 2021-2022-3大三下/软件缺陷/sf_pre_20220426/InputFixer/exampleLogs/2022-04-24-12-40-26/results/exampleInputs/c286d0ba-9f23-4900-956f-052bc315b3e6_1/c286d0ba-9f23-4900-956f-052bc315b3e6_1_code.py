a, x, b = [], 0, []
while True:
    b = ([str(j) for j in input().split()])
    if b[0] == 'end':
        break
    for i in range(len(b)):
        b[i] = int(b[i])
    a.insert(x, b)
    x +=1
sum = [[0 for j in range(len(a[i]))] for i in range(len(a))]
for i in range(len(a)):
    for j in range(len(a[i])):
        c, d, g, h = 0, 0, 0, 0
        if (len(a) -1) == 0:
            g = 1
        elif i == (len(a) - 1):
            c = -3
        if (len(a[i]) - 1) == 0:
            h = 1
        elif j == (len(a[i]) - 1):
            d = -3            
        sum[i][j] = ((a[i - 1+g][j]) + (a[i +1-g+c][j]) + (a[i][j-1+h]) + (a[i][j+1-h+d]))
        sum[i][j] = (sum[i - 1+g][j])
        print(sum[i - 1+g][j])
        print(sum[i][j])        
        sum[i][j] += (sum[i +1-g+c][j])
        print(sum[i +1-g+c][j])
        print(sum[i][j])  
        sum[i][j] += (sum[i][j-1+h])
        print(sum[i][j-1+h])
        print(sum[i][j])  
        sum[i][j] += (sum[i][j+1-h+d])
        print(sum[i][j+1-h+d])
        print(sum[i][j])  
for i in range(len(sum)):
    for j in range(len(sum[i])):
        print(sum[i][j], end=' ')
    print()