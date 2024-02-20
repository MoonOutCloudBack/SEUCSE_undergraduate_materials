n=int(input())
vetor=list(map(str,input().split()))
m1,m2=map(str,input().split())
for e in vetor:
	if m1>e>m2 or m1<e<m2:
		print(e,end=' ')
		
