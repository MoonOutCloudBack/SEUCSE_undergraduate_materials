notas=input()
n1,n2,n3,n4=notas.split(' ')
n1=float(n1)
n2=float(n2)
n3=float(n3)
n4=float(n4)
media=(n1*2+n2*3+n3*4+n4*1)/10
print("Media: %.1f" % media)
if media>=7.0:
	print("Aluno aprovado.")
elif media<5:
	print("Aluno reprovado.")
else:
	print("Aluno em exame.")
	rec=float(input())
	print("Nota do exame: %.1f" % rec)
	nova_media=(rec+media)/2
	if nova_media>=5:
		print("Aluno aprovado.")
	else:
		print("Aluno reprovado.")
	print("Media final: %.1f" % nova_media)