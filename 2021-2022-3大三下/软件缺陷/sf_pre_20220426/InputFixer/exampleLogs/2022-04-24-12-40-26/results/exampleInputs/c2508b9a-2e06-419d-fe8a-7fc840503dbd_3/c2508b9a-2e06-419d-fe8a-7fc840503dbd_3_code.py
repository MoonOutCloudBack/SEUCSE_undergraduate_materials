elementos=input()
n1,n2,n3=elementos.split(' ')
n1=float(n1)
n2=float(n2)
n3=float(n3)
lista=[n1,n2,n3]
lista.sort(reverse=True)
if lista[0]>0 and lista[1]>0 and lista[2]:
    if lista[0]>=lista[1]+lista[2]:
    	print("NAO FORMA TRIANGULO")
    elif lista[0]**2==(lista[1]**2+lista[2]**2):
        print("TRIANGULO RETANGULO")
    elif lista[0]**2>lista[1]**2+lista[2]*2:
        print("TRIANGULO OBTUSANGULO")
    else:
        print("TRIANGULO ACUTANGULO")
        
    if lista[0]==lista[1]==lista[2]:
        print("TRIANGULO EQUILATERO")
    else:
        print("TRIANGULO ISOSCELES")
        