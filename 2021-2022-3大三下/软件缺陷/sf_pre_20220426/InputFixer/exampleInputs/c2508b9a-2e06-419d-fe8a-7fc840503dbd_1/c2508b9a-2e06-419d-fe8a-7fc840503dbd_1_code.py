elementos=input()
n1,n2,n3=elementos.split(' ')
n1=float(n1)
n2=float(n2)
n3=float(n3)
if n1+n2>n3 and n1+n3>n2 and n2+n3>n1:
	print("Perimetro = %2.1f" % (n1+n2+n3))
else:
	print("Area = %2.1f" % ((n3*(n1+n2))/2))