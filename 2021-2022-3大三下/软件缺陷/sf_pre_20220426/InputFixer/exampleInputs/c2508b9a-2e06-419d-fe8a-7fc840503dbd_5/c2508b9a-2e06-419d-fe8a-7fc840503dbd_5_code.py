hora=input()

n1,n2=hora.split(' ')

n1=int(n1)

n2=int(n2)



if 1>=abs(n1-n2)>=24:
	print("O JOGO DUROU %d HORA(S)" % (abs(n1-n2)))