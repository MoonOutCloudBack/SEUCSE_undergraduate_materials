nome = input().upper()
funcionarios_cadastrados = {'ANA':1111, 'CAIO':2222, 
'JEFERSON':3333, 'NATHALIA':4444, 'NOME':5555}
if nome.upper() in funcionarios_cadastrados:
    pass
elif nome.uper() not in funcionarios_cadastrados:
    nome = input().lower()
    CPF = int(input())
    funcionarios_cadastrados[CPF] = nome
    