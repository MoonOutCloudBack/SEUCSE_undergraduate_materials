s = input()
n = 8
t = s[n:]
if len(t) > 0:
    while t[0] == "n":
        n += 1
        t = s[n:]