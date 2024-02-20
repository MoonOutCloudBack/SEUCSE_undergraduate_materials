n = int ( input ())
a = []
for i in range (n):
    a.append ( [0]* n)
print (a) # лишняя, потом удалить
N = 1
i = 0
j = 0
m = n
v = 'вправо'
while N <= n ** 2:
    while v == 'вправо':
        a [i][j] = N
        N += 1
        if N > 25:
            break
        j += 1
        if j == m :
            j -= 1
            i += 1
            v = 'вниз'
            break
    while v == 'вниз':
        a [i][j] = N
        N += 1
        if N > 25:
            break
        i += 1
        if i == m :
            v = 'влево'
            i -= 1
            j -= 1
            break
    while v == 'влево':
        a [i][j] = N
        N += 1
        if N > 25:
            break
        if j == n - m:
            v = 'вверх'
            m -= 1
            i -= 1
            break
        j -= 1
    while v == 'вверх':
        a [i][j] = N
        N += 1
        if N > 25:
            break
        if i == n - m:
            v = 'вправо'
            j += 1
            break
        i -= 1
for i in a:
    print (i)