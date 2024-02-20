class Serie:
    titulo=0
    numerotemp=0
    genero=" "
    

cont=0
for i in range (0,3):
    a=Serie()
    a.titulo=int(input("ingrese el titulo"))
    a.numerotemp=int(input("ingrese el numero de tempo"))
    a.genero=input("ingrese el genero")
    cont=cont+a.numerotemp
print(cont)
    
