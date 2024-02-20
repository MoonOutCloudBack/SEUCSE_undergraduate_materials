matriz = []
cont=0
for i in range(3):
  linha =[]
  for j in range(3):
    linha.append(int(input()))
  matriz.append(linha)
for i in range(3):
  if linha[0] or linha[1] or linha[2]==0:
    cont+=1
for i in range(3):
  print(matriz[i])
print(cont)